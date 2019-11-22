#define FROM_DABBLE_LIBRARY
#include "DabbleESP32.h"
#include "SensorModule.h"

SensorModule::SensorModule(): ModuleParent(SENSORS_ID)
{

}

float SensorModule::getAccelerometerXaxisData()
{
return accelo_x;
}

float SensorModule::getAccelerometerYaxisData()
{
return accelo_y;
}

float SensorModule::getAccelerometerZaxisData()
{
return accelo_z;
}


float SensorModule::getGyroscopeXaxisData()
{
return gyro_x;
}

float SensorModule::getGyroscopeYaxisData()
{
return gyro_y;
}

float SensorModule::getGyroscopeZaxisData()
{
return gyro_z;
}


float SensorModule::getMagnetometerXaxisData()
{
return magneto_x;
}

float SensorModule::getMagnetometerYaxisData()
{
return magneto_y;
}

float SensorModule::getMagnetometerZaxisData()
{
return magneto_z;
}


float SensorModule::getProximityData()
{
return proximity;
}

float SensorModule::getLightData()
{
return light;
}

float SensorModule::getSoundData()
{
return sound_level;
}



float SensorModule::getTemperatureData()
{
return temperature;
}

float SensorModule::getBarometerData()
{
return barometer;
}

float SensorModule::getGPSLongitudeData()
{
return gps_longitude;
}

float SensorModule::getGPSLatitude()
{
return gps_latitude;
}

float SensorModule::getSensorData(uint8_t a)
{
	if(a==0)
	{
		return accelo_x;
	}
	else if(a==1)
	{
		return accelo_y;
	}
	else if(a==2)
	{
		return accelo_z;
	}
	else if(a==3)
	{
		return gyro_x;
	}
	else if(a==4)
	{
		return gyro_y;
	}
	else if(a==5)
	{
		return gyro_z;
	}
	else if(a==6)
	{
		return magneto_x;
	}
	else if(a==7)
	{
		return magneto_y;
	}
	else if(a==8)
	{
		return magneto_z;
	}
	else if(a==9)
	{
		return proximity;
	}
	else if(a==10)
	{
		return temperature;
	}
	else if(a==11)
	{
		return sound_level;
	}
	else if(a==12)
	{
		return barometer;
	}
	else if(a==13)
	{
		return gps_longitude;
	}
	else if(a==14)
	{
		return gps_latitude;
	}
	else if(a==15)
	{
		return light;
	}
}

void SensorModule::processData()
{
	byte functionId =getDabbleInstance().getFunctionId();
	if(functionId == ACCELEROMETER || functionId == GYROSCOPE || functionId == MAGNETOMETER)
	{
        if(functionId == ACCELEROMETER)
		{
			accelo_x = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
			accelo_y = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(1));
			accelo_z = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(2));
		}
		else if(functionId == GYROSCOPE)
		{
			gyro_x = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
			gyro_y = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(1));
			gyro_z = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(2));
		}
		else if(functionId == MAGNETOMETER)
		{
			magneto_x = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
			magneto_y = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(1));
			magneto_z = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(2));
		}
	}
	
	if(functionId == PROXIMITY || functionId == TEMPERATURE || functionId == LIGHT || functionId == SOUND || functionId == BAROMETER)
	{
		if(functionId == PROXIMITY )
		{
			proximity = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
		}
		else if(functionId == TEMPERATURE)
		{
			temperature = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
		}
			else if(functionId == SOUND)
		{
			sound_level = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
		}
			else if(functionId == BAROMETER)
		{
			barometer = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
		}	
		else if(functionId == LIGHT)
		{
			light = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
		}
	}
	
	if(functionId == GPS)
		{
			
			gps_longitude = getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(1));
			gps_latitude =  getDabbleInstance().convertBytesToFloat(getDabbleInstance().getArgumentData(0));
		}	
	}

