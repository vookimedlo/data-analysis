#pragma once
/****************************************************************************
Data Analysis - tool for making a basic data analysis.
Copyright(C) 2017  Michal Duda <github@vookimedlo.cz>

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include <map>

// Note - Michal Duda: idea got from http://stackoverflow.com/questions/13592847/c11-observer-pattern-signals-slots-events-change-broadcaster-listener-or

template<typename Func>
struct CSignal {
    typedef int Key;
    Key nextKey;
    std::map<Key, Func> connections;

    template<typename FuncLike>
    Key connect(FuncLike f) {
        Key k = nextKey++;
        connections[k] = f;
        return k;
    }

    void disconnect(Key k) {
        connections.erase(k);
    }

    size_t registeredObservers() const {
        return connections.size();
    }

    template<typename ...Args>
    typename Func::result_type call(Args... args) {
        for (auto &connection : connections) {
            (connection.second)(std::forward<Args>(args)...);
        }
    }
};
