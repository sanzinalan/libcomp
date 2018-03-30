/**
 * @file libcomp/src/ScriptEngine.h
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Class to manage Squirrel scripting.
 *
 * This file is part of the COMP_hack Library (libcomp).
 *
 * Copyright (C) 2012-2018 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBCOMP_SRC_SCRIPTENGINE_H
#define LIBCOMP_SRC_SCRIPTENGINE_H

// libcomp Includes
#include "CString.h"

// Squirrel Includes
#include <squirrel.h>
#include <sqstdblob.h>

// Sqrat includes
#include "PushIgnore.h"
#include <sqrat.h>
#include "PopIgnore.h"

// Standard C++11 Includes
#include <set>

namespace libcomp
{

/**
 * Represents a Sqrat based Squirrel virtual machine handler to facilitate
 * script execution and bind @ref Object instances to the VM.
 */
class ScriptEngine
{
public:
    /**
     * Create the VM.
     * @param useRawPrint Set this to not prefix messages with "SQUIRREL: ".
     */
    ScriptEngine(bool useRawPrint = false);

    /**
     * Clean up the VM.
     */
    ~ScriptEngine();

    /**
     * Signifies that the templated type will be used in the VM instance.
     * Implementations of this function are generated by objgen when the
     * object is flagged as "scriptEnabled". Multiple calls to this function
     * will not bind the object multiple times.
     * @return Reference back to the engine so they can be chained
     */
    template <class T> ScriptEngine& Using();

    /**
     * Get the Sqrat VM.
     * @return The Sqrat VM
     */
    HSQUIRRELVM GetVM();

    /**
     * Evaluate a Squirrel script block as a string.
     * @param source Squirrel script block as a string
     * @return true on success, false on failure
     */
    bool Eval(const String& source, const String& sourceName = String());

private:
    /**
     * Utility function to complete the binding of an object via @ref ScriptEngine::Using.
     * @param name The name of the binding which should match a class name
     * @param binding The object binding
     */
    template <class T> void Bind(const std::string& name,  Sqrat::Class<T>& binding)
    {
        mBindings.insert(name);
        Sqrat::RootTable(mVM).Bind(name.c_str(), binding);
    }

    /**
     * Check if an object has already been bound.
     * @param name The name of the binding to check
     * @param lockBinding Optional parameter to mark the binding as known
     *  now instead of waiting until calling @ref ScriptEngine::Bind. Useful
     *  for static functions or when circular references can occur.
     * @return true if it was bound already, false if it was not
     */
    bool BindingExists(const std::string& name, bool lockBinding = false);

    /// The Sqrat VM
    HSQUIRRELVM mVM;

    /// Bindings that have already been made to objects via @ref ScriptEngine::Using
    std::set<std::string> mBindings;
};

} // namespace libcomp

#endif // LIBCOMP_SRC_SCRIPTENGINE_H
