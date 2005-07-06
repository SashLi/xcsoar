/*
XCSoar Glide Computer
Copyright (C) 2000 - 2004  M Roberts

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
*/
#include "stdafx.h"

#include "Process.h"
#include "externs.h"
#include "Utils.h"
#include "device.h"

// JMW added key codes,
// so -1 down
//     1 up
//     0 enter

void	AltitudeProcessing(int UpDown)
{
	#ifdef _SIM_
		if(UpDown==1)
			GPS_INFO.Altitude += (100/ALTITUDEMODIFY);
		else if (UpDown==-1)
		{
			GPS_INFO.Altitude -= (100/ALTITUDEMODIFY);
			if(GPS_INFO.Altitude < 0)
				GPS_INFO.Altitude = 0;
		} else if (UpDown==-2) {
			DirectionProcessing(-1);
		} else if (UpDown==2) {
			DirectionProcessing(1);
		}
	#endif
	return;
}

void	SpeedProcessing(int UpDown)
{
	#ifdef _SIM_
		if(UpDown==1)
			GPS_INFO.Speed += (10/SPEEDMODIFY);
		else if (UpDown==-1)
		{
			GPS_INFO.Speed -= (10/SPEEDMODIFY);
			if(GPS_INFO.Speed < 0)
				GPS_INFO.Speed = 0;
		} else if (UpDown==-2) {
			DirectionProcessing(-1);
		} else if (UpDown==2) {
			DirectionProcessing(1);
		}
	#endif
	return;
}

void	WindDirectionProcessing(int UpDown)
{
	
	if(UpDown==1)
	{
		CALCULATED_INFO.WindBearing  += 5;
		while (CALCULATED_INFO.WindBearing  >= 360)
		{
			CALCULATED_INFO.WindBearing  -= 360;
		}
	}
	else if (UpDown==-1)
	{
		CALCULATED_INFO.WindBearing  -= 5;
		while (CALCULATED_INFO.WindBearing  < 0)
		{
			CALCULATED_INFO.WindBearing  += 360;
		}
	} else if (UpDown == 0) {
	  SaveWindToRegistry();
	}
	return;
}


void	WindSpeedProcessing(int UpDown)
{
	if(UpDown==1)
		CALCULATED_INFO.WindSpeed += (1/SPEEDMODIFY);
	else if (UpDown== -1)
	{
		CALCULATED_INFO.WindSpeed -= (1/SPEEDMODIFY);
		if(CALCULATED_INFO.WindSpeed < 0)
			CALCULATED_INFO.WindSpeed = 0;
	} 
	// JMW added faster way of changing wind direction
	else if (UpDown== -2) {
		WindDirectionProcessing(-1);
	} else if (UpDown== 2) {
		WindDirectionProcessing(1);
	} else if (UpDown == 0) {
	  SaveWindToRegistry();
	}
	return;
}

void	DirectionProcessing(int UpDown)
{
	#ifdef _SIM_
	
		if(UpDown==1)
		{
			GPS_INFO.TrackBearing   += 5;
			while (GPS_INFO.TrackBearing  >= 360)
			{
				GPS_INFO.TrackBearing  -= 360;
			}
		}
		else if (UpDown==-1)
		{
			GPS_INFO.TrackBearing  -= 5;
			while (GPS_INFO.TrackBearing  < 0)
			{
				GPS_INFO.TrackBearing  += 360;
			}
		}
	#endif
	return;
}

void	McReadyProcessing(int UpDown)
{

	if(UpDown==1) {
		MACREADY += (double)0.2;

		if (MACREADY>10.0) { // JMW added sensible limit
			MACREADY=10.0;
		}
	}
	else if(UpDown==-1)
	{
		MACREADY -= (double)0.2;
		if(MACREADY < 0)
		{
			MACREADY = 0;
		}
	} else if (UpDown==0)
	{
		CALCULATED_INFO.AutoMcReady = !CALCULATED_INFO.AutoMcReady; // JMW toggle automacready
	}

  devPutMcReady(devA(), MACREADY);
  devPutMcReady(devB(), MACREADY);

	return;
}

extern void PopupWaypointDetails();

void NextUpDown(int UpDown)
{
	if(UpDown==1)
	{
		if(ActiveWayPoint < MAXTASKPOINTS)
		{
			if(Task[ActiveWayPoint+1].Index >= 0)
			{
				if(ActiveWayPoint == 0)
				{
					CALCULATED_INFO.TaskStartTime = GPS_INFO.Time ;
				}
				ActiveWayPoint ++;
				CALCULATED_INFO.LegStartTime = GPS_INFO.Time ;
			}
		}
	}
	else if (UpDown==-1)
	{
		if(ActiveWayPoint >0)
		{
			ActiveWayPoint --;
		}
	} else if (UpDown==0) {
		SelectedWaypoint = Task[ActiveWayPoint].Index;
		UnlockFlightData();
		PopupWaypointDetails();
		LockFlightData();
	}
}


void NoProcessing(int UpDown)
{
	return;
}
