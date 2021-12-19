//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

#include "map_in_sld.h"

// XMidiCtrl
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_sld::map_in_sld(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_sld::type()
{
    return map_type::slider;
};


/**
 * Set the up command
 */
void map_in_sld::set_command_up(std::string_view command)
{
    m_command_up = command;
}


/**
 * Return the up command
 */
std::string_view map_in_sld::command_up() const
{
    return m_command_up;
}


/**
 * Set the middle command
 */
void map_in_sld::set_command_middle(std::string_view command)
{
    m_command_middle = command;
}


/**
 * Return the middle command
 */
std::string_view map_in_sld::command_middle() const
{
    return m_command_middle;
}


/**
 * Set the down command
 */
void map_in_sld::set_command_down(std::string_view command)
{
    m_command_down = command;
}


/**
 * Return the down command
 */
std::string_view map_in_sld::command_down() const
{
    return m_command_down;
}


/**
 * Set the dataref
 */
void map_in_sld::set_dataref(std::string_view dataref)
{
    m_dataref = dataref;
}


/**
 * Return the int dataref
 */
std::string_view map_in_sld::dataref() const
{
    return m_dataref;
}

/**
 * Set the array index
 */
void map_in_sld::set_index(std::string_view index)
{
    if (!index.empty())
        m_index = std::stoi(index.data());
}


/**
 * Set the offset
 */
void map_in_sld::set_offset(std::string_view offset)
{
    if (!offset.empty())
        m_offset = std::stof(offset.data());
}


/**
 * Set the scale
 */
void map_in_sld::set_scale(std::string_view scale)
{
    if (!scale.empty())
        m_scale = std::stof(scale.data()) / 127;
}


/**
 * Read settings from config
 */
void map_in_sld::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'sld'" << LOG_END
    map::read_config(settings);

    // read command up
    set_command_up(utils::read_string_parameter(settings, CFG_KEY_COMMAND_UP));

    // read command middle
    set_command_middle(utils::read_string_parameter(settings, CFG_KEY_COMMAND_MIDDLE));

    // read command down
    set_command_down(utils::read_string_parameter(settings, CFG_KEY_COMMAND_DOWN));

    // read dataref_int
    set_dataref(utils::read_string_parameter(settings, CFG_KEY_DATAREF));

    // read array index
    set_index(utils::read_string_parameter(settings, CFG_KEY_INDEX));

    // read offset
    set_offset(utils::read_string_parameter(settings, CFG_KEY_OFFSET));

    // read scale
    set_scale(utils::read_string_parameter(settings, CFG_KEY_SCALE));
}


/**
 * Check the mapping
 */
bool map_in_sld::check()
{
    if (!map::check())
        return false;

    //    if (!m_dataref.empty() && (!m_command_up.empty() || !m_command_down.empty()))
    //       return false;

	//    return !(m_command_up.empty() && m_command_down.empty());
	return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_sld::execute(midi_message &msg)
{
    if (m_dataref.length() > 0) {
      m_xp->datarefs().write(m_dataref, m_index, m_offset, m_scale, (float)msg.velocity / (float)127);
    } else if (msg.velocity <= 10) {
        LOG_DEBUG << " --> Execute command '" << m_command_down << "'" << LOG_END
        m_xp->cmd().execute(m_command_down);
    } else if (msg.velocity >= 117) {
        LOG_DEBUG << " --> Execute command '" << m_command_up << "'" << LOG_END
        m_xp->cmd().execute(m_command_up);
    } else if (msg.velocity >= 50 && msg.velocity <= 70) {
        if (!m_command_middle.empty()) {
            LOG_DEBUG << " --> Execute command '" << m_command_middle << "'" << LOG_END
            m_xp->cmd().execute(m_command_middle);
        }
    }
}

} // Namespace xmidictrl
