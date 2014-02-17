/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2013 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Device/Driver/OpenVario.hpp"
#include "Device/Driver.hpp"
#include "NMEA/Checksum.hpp"
#include "NMEA/Info.hpp"
#include "NMEA/InputLine.hpp"

class OpenVarioDevice : public AbstractDevice {
public:
  virtual bool ParseNMEA(const char *line, NMEAInfo &info) override;

  static bool POV(NMEAInputLine &line, NMEAInfo &info);
};

bool
OpenVarioDevice::ParseNMEA(const char *_line, NMEAInfo &info)
{
  if (!VerifyNMEAChecksum(_line))
    return false;

  NMEAInputLine line(_line);
  if (line.ReadCompare("$POV"))
    return POV(line, info);

  return false;
}

bool
OpenVarioDevice::POV(NMEAInputLine &line, NMEAInfo &info)
{
  /*
   * Type definitions:
   *
   * P: static pressure in hPa
   * Q: dynamic pressure in Pa
   */

  while (!line.IsEmpty()) {
    char type = line.ReadOneChar();
    if (type == '\0')
      break;

    fixed value;
    if (!line.ReadChecked(value))
      break;

    switch (type) {
      case 'P': {
        AtmosphericPressure pressure = AtmosphericPressure::HectoPascal(value);
        info.ProvideStaticPressure(pressure);
        break;
      }
      case 'Q': {
        AtmosphericPressure pressure = AtmosphericPressure::Pascal(value);
        info.ProvideDynamicPressure(pressure);
        break;
      }
    }
  }

  return true;
}

static Device *
OpenVarioCreateOnPort(const DeviceConfig &config, Port &com_port)
{
  return new OpenVarioDevice();
}

const struct DeviceRegister open_vario_driver = {
  _T("OpenVario"),
  _T("OpenVario"),
  0,
  OpenVarioCreateOnPort,
};
