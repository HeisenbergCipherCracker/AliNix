/**
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/
/**
 * @abstraction:
 *  - Provides the system component utility.
*/
#include <alinix/system/component.h>
#include <alinix/module.h>

MODULE_AUTHOR("Ali Mirmohammad")
MODULE_DESCRIPTION("System component utility")
MODULE_LICENSE("AGPL-3.0")

SystemComponent* SystemComponent_create(char* name, char* description) {
    SystemComponent* component = (SystemComponent*)malloc(sizeof(SystemComponent));
    component->Name = (char*)malloc(strlen(name) + 1);
    strcpy(component->Name, name);
    component->Description = (char*)malloc(strlen(description) + 1);
    strcpy(component->Description, description);
    return component;
}