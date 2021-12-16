# encase_challenge

Requirements - Remote environmental monitoring solution. 

Challengers must build projects for a hazardous environment, such as extreme cold and heat or exposure to water and humidity. Your project could be designed for the industrial environment, outdoor monitoring, food production areas, and more.

High Level Requirements

Ascertain if any nearby The Things Network gateways (if so, consider vs. at home solution)
Must provide a remote environmental monitoring solution
Must explain / provide rationale for each sensed data point and the hardware selection
Must be weather proof and make use of a Hammond Manufacturing Enclosure
Must demonstrate the weatherproofness of the enclosure
	Potential submergence testing prior to deployment
	Without cable glands, 100% sealed test
	With cable glands and sealed stubs, for sealed test
Must be capable of being battery or mains powered
Must provide the defined monitoring parameters - for water and environment
Must use Lora connectivity, no gateway but point to point. 
Must be built around the Arduino MKR 1300 (see above requirement)
Needs to use a 3.3v input for the Arduino 
Must be in place for sufficent time to record data
Must demonstrate solution 
Must provide data connectivity to store recorded values
Must provide sample data every n-minutes - provide rationale on sample period selection, also work through storage reqs
Must provide data visualisation of parameters recorded
Must provide code for final solution
Must provide build instructions 
IF home solution, must provide detailed design and deployment information (to be repeatable)

Solution specification

Monitoring parameters

Must be capable of monitoring and providing absolute values for: 

Air temperature
Humidity
Air pressure 
Water pH
Water turbidity
Total Disolved Solids (TDS)
Water temperature (potentially at surface and at depth)

Potential Solution specification additions

Ambient light level measurement, LUX
Oxidisation Reduction Potential (ORP) if i can find a vendor with any stock that isnt £140


Air temperature
Humidity

or 

Air temperature + Humidity in a single package, 

Air pressure 

Water pH

Water turbidity - Gravity: Analog Turbidity Sensor 

Total Disolved Solids (TDS) - Gravity: Analog TDS Sensor

Water temperature (potentially at surface and at depth)


Ambient light level measurement, LUX - Adafruit VEML7700 Lux Sensor 
