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

#ifndef MAP_OUT_DRF_H
#define MAP_OUT_DRF_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_out.h"
#include "midi_message.h"

namespace xmidictrl {

class map_out_drf : public map_out {
public:
    explicit map_out_drf(std::shared_ptr<xplane> xp);
    ~map_out_drf() override = default;

    map_type type() override;

    void set_dataref(std::string_view dataref);
    [[nodiscard]] std::string_view dataref() const;

    void set_value_on(std::string_view value_on);
    [[nodiscard]] std::string_view value_on() const;

    void set_value_off(std::string_view value_off);
    [[nodiscard]] std::string_view value_off() const;

    void read_config(toml::value &settings) override;
    bool check() override;

    std::shared_ptr<midi_message> execute() override;

private:
    std::string m_dataref;
    std::string m_value_on;
    std::string m_value_off;

    std::string m_value_previous;
};

} // Namespace xmidictrl

#endif // MAP_OUT_DRF_H