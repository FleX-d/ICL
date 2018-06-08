#INSTALL 

sudo apt-get install xsltproc_1.1.28-2+deb8u3_amd64.deb

#RUN ./build.sh test.xml test1.xml

#Example of xml file 

<?xml version = "1.0"?>
<?xml-stylesheet type="text/xsl" href="transform.xsl"?>                     ------------> required name of stylesheet "transform.xsl" and type of output "text/xsl"
<package name="Interface">                                                  ------------> name irelevant
  
    <element name="HelloMsgParameters">                                     ------------> Additional Header parameters, have to be write all of them always
	<parameter name="msgtype" type="uint8_t" value="1"/>                ------------> name of parameter, type and value required
	<parameter name="msgCounter" type="uint8_t" value="m_counter"/>     ------------> There is a implement counter 
	<parameter name="crc" type="uint16_t" value="NAN"/>		    ------------> If Value = NAN ->parameter is not used				
	<parameter name="timeStamp" type="uint32_t" value="getTimestamp()"/>  ----------> There is a implement method for timeStamp
	<parameter name="from" type="uint32_t" value="21322"/>
	<parameter name="to" type="uint32_t" value="54483"/>
	<parameter name="nextHeader" type="uint8_t" value="NAN"/>
	<parameter name="reserved" type="uint8_t" value="NAN"/>
    </element>
    
    <function name="HelloMsg" direction="in">                               ------------> Name of function and direction are required |direction = in -> message will be send form client and receive by server|
	<parameter name="id" type="int" value="2" direction="in"/>	    ------------> Direction in ---> parameter will not be atribute of function, will be stored at payload /@name, just for internal information bettween interfaces
	<parameter name="context" type="std::string" direction="out"/>      ------------> Direction out ---> parameter will be atribute of function, will be stored at payloadl /payload/@name, for external usecase
	<parameter name="nieco" type="uint8_t" direction="out"/>
    </function>
    
    <function name="HelloMsgAck" direction="out">                           ------------> Name of function and direction are required |direction = out -> message will be send form server and receive by client|
	<parameter name="id" type="int" value="2" direction="in"/>          ------------> Direction in ---> parameter will not be atribute of function, will be stored at payload /@name, just for internal information bettween interfaces
	<parameter name="context" type="std::string" direction="out"/>      ------------> Direction out ---> parameter will be atribute of function, will be stored at payloadl /payload/@name, for external usecase
	<parameter name="nieco" type="uint8_t" direction="out"/>
    </function>
</package>
