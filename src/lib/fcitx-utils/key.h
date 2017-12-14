/*
 * Copyright (C) 2015~2015 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */

#ifndef _FCITX_UTILS_KEY_H_
#define _FCITX_UTILS_KEY_H_

/// \addtogroup FcitxUtils
/// \{
/// \file
/// \brief Class to represent a key.

#include "fcitxutils_export.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fcitx-utils/flags.h>
#include <fcitx-utils/keysym.h>
#include <string>
#include <vector>

namespace fcitx {
class Key;
typedef FcitxKeySym KeySym;
typedef Flags<KeyState> KeyStates;
typedef std::vector<Key> KeyList;

class FCITXUTILS_EXPORT Key {
public:
    explicit Key(KeySym sym = FcitxKey_None, KeyStates states = KeyStates(),
                 int code = 0)
        : sym_(sym), states_(states), code_(code) {}

    explicit Key(const char *keyString);
    explicit Key(const std::string &keyString) : Key(keyString.c_str()) {}

    FCITX_INLINE_DEFINE_DEFAULT_DTOR_COPY_AND_MOVE(Key)

    static Key fromKeyCode(int code = 0, KeyStates states = KeyStates()) {
        return Key(FcitxKey_None, states, code);
    }

    bool operator==(const Key &key) const {
        return sym_ == key.sym_ && states_ == key.states_ && code_ == key.code_;
    }

    bool check(const Key &key) const;
    bool check(KeySym sym = FcitxKey_None,
               KeyStates states = KeyStates()) const {
        return check(Key(sym, states));
    }
    bool isDigit() const;
    bool isUAZ() const;
    bool isLAZ() const;
    bool isSimple() const;
    bool isModifier() const;
    bool isCursorMove() const;
    bool hasModifier() const;
    Key normalize() const;

    std::string toString() const;
    std::string toDisplayString() const;

    bool isValid() const;

    inline KeySym sym() const { return sym_; }
    inline KeyStates states() const { return states_; }
    inline int code() const { return code_; }

    static KeyStates keySymToStates(KeySym sym);
    static KeySym keySymFromString(const std::string &keyString);
    static std::string keySymToString(KeySym sym);
    static std::string keySymToDisplayString(KeySym sym);
    static KeySym keySymFromUnicode(uint32_t unicode);
    static uint32_t keySymToUnicode(KeySym sym);
    static std::string keySymToUTF8(KeySym sym);
    static KeyList keyListFromString(const std::string &str);
    template <typename Container>
    static std::string keyListToString(Container container) {
        std::string result;
        bool first = true;
        for (auto k : container) {
            if (first) {
                first = false;
            } else {
                result += " ";
            }
            result += k.toString();
        }
        return result;
    }
    template <typename Container>
    bool checkKeyList(const Container &c) {
        return std::find_if(c.begin(), c.end(), [this](const Key &toCheck) {
                   return check(toCheck);
               }) != c.end();
    }

    template <typename Container>
    int keyListIndex(const Container &c) {
        size_t idx = 0;
        for (auto &toCheck : c) {
            if (check(toCheck)) {
                break;
            }
            idx++;
        }
        if (idx == c.size()) {
            return -1;
        }
        return idx;
    }

private:
    KeySym sym_;
    KeyStates states_;
    int code_;
};
}

#endif //  _FCITX_UTILS_KEY_H_
