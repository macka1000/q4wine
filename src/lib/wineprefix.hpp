/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <regex>

#include "src/lib/defines.hpp"
#include "src/lib/dbobject.hpp"
#include "src/lib/wineversion.hpp"
#include "src/lib/wineapplication.hpp"

namespace q4wine {
namespace lib {

const std::string DEFAULT_EXEC_TEMPLATE = "%CONSOLE_BIN% %CONSOLE_ARGS% " \
                    "%ENV_BIN% %ENV_ARGS% " \
                    "%SET_NICE% %WINE_BIN% " \
                    "%VIRTUAL_DESKTOP% %PROGRAM_BIN% %PROGRAM_ARGS% 2>&1";

/*! \class WinePrefix wineprefix.hpp <q4wine/src/lib/wineprefix.hpp>
 * \brief Describes a wine prefix configuration.
 *
 * \par Wine Terms
 *
 * In terms of Wine WINEPREFIX is a directory which holds a virtual
 * windows drive with separate wine settings (drives, virtual desktop,
 * special dlls and so on).
 *
 * \par Q4Wine Terms
 *
 * In terms of Q4Wine WinePrefix is a set of options which describe basic
 * WINEPREFIX configuration.
 *
 * \note It is possible to have multiple WinePrefix pointed to the single
 * directory. In combination with WineVersion: setups is possible to run a
 * number of truly independent wine processes.
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class WinePrefix : public DBObject {
 public:
    /*! Constructs an empty WinePreifx object. */
    WinePrefix();
    /*! Constructs an WinePreifx object.
     *
     * \param name Is user defined name, should be unique.
     * See also: #setName #getName
     *
     * \param path The name of the directory where Wine will store its data.
     * See also: #setPath #getPath
     *
     * \param arch Is the architecture supported by a given Wine prefix. It
     * is set at prefix creation time and cannot be changed afterwards without
     * wiping prefix directory content.
     * See also: #setArch #getArch
     *
     * \param version An WineVersion instance.
     * See alse: #setVersion #getVersion
     *
     * \param mountPoint An mount point directory where Disk images or
     * CD/DVD drives will be mounted by user request.
     * See also: #setMountPoint #getMountPoint
     *
     * \param virtualDrive An virtual drive letter for mount point.
     * See also: #setVirtualDrive #getVirtualDrive
     *
     * \param execTemplate Will be used to format command line string to run wine
     * programs.
     * See alse: #setExecutionTemplate #getExecutionTemplate
     *
     * \param id The associated row id. If it set to 0, then the object
     * is not associated with any database record.
     * See also: DBObject#setId DBObject#getId
    */
    WinePrefix(std::string name,
            std::string path,
            std::string arch,
            WineVersion* version,
            std::string mountPoint = std::string(),
            std::string virtualDrive = std::string(),
            std::string execTemplate = DEFAULT_EXEC_TEMPLATE,
            uintptr_t id = 0);
    /*! Destroys this WinePrefix object. */
    ~WinePrefix();

    /*!
     * \brief Object into DB.
     * \return True on success.
     */
    virtual bool save(void);

    /*!
     * \brief Create WinePrefix object from DB by version.id.
     * \param id Record id.
     * \return WinePrefix object or NULL if not found.
     */
    static WinePrefix* getInstance(intptr_t id);

    /*! Constructs wine env variables (WINEPREFIX, WINESERVER, WINELOADER,
     * WINEDLLPATH, WINEARCH) using WinePrefix and WineVersion data.
     *
     * Example:
     * WINEPREFIX='/mnt/ssd/wine/'
     * WINESERVER='/usr/bin/wineserver'
     * WINELOADER='/usr/bin/wine'
     * WINEDLLPATH='/usr/lib64/wine/'
     * WINEARCH='win64'
     *
     * \return Formatted env variables
     */
    std::string getEnvVariables(const WineApplication *wineApp);

    /*! Constructs execution string from temaplteString using WineAppliction
     * data.
     *
     * \param wineApp An reference to WineApplication instace.
     *
     * \return Execution string.
     */
    std::string getExecutionString(const WineApplication* wineApp);

    void setName(std::string name);
    void setPath(std::string path);
    void setArch(std::string arch);
    void setVersion(WineVersion* version);
    void setMountPoint(std::string mountPoint);
    void setVirtualDrive(std::string virtualDevice);
    void setExecutionTemplate(std::string execTemplate);
    const std::string getName(void) const;
    const std::string getPath(void) const;
    const std::string getArch(void) const;
    WineVersion* getVersion(void) const;
    const std::string getMountPoint(void) const;
    const std::string getVirtualDrive(void) const;
    const std::string getExecutionTemplate(void) const;

 private:
    /*! Set the table name in q4wine database to lookup object data */
    static const std::string tableName_;
    std::string name_;
    std::string path_;
    std::string arch_;
    std::shared_ptr<WineVersion> version_;
    std::string mountPoint_;
    std::string virtualDrive_;
    std::string execTemplate_;
};

}  // namespace lib
}  // namespace q4wine
