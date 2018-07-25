## Genarator
Generator is aplication written at XSLT language, which is using XML files to generate text output

## Install
Command for install xslt pre-procesor 

    sudo apt-get install xsltproc_1.1.28-2+deb8u3_amd64.deb

## Start Script
Command for start script: 

    ./build.sh 88881 MCAppComGen.xml 

 88881 - is ID of your application and has to be written also at XML file at position package/element/parameter/name[from]/value
 
 MCAppComGen.xml - is XML file which will be processing. 
 After ID can be written more files, for examlpe: ./build.sh 88881 MCAppComGen.xml MCAppCom.xml

If you want to generate two interfaces for each side and this interfaces will be in reverse use this commands.

    1.  ./build.sh 00000 MCAppComGen.xml MCAppCom.xml
    2.  ./build.sh 88881 MCAppComGen.xml MCAppCom.xml
    
Each command generates one .h .cpp file. If this files exist pre-procesor wil override them. 
 
## Example of xml file 

    <?xml version = "1.0"?>
    <?xml-stylesheet type="text/xsl" href="transform.xsl"?> 
    <package name="IPCInterface" generic="true">         
    
      <element name="HelloMsgParameters">    
	<parameter name="msgtype" type="uint8_t" value="1"/>              
	<parameter name="msgCounter" type="uint8_t" value="m_counter"/>    
	<parameter name="crc" type="uint16_t" value="NAN"/>    
	<parameter name="timeStamp" type="uint32_t" value="getTimestamp()"/>  
	<parameter name="from" type="uint32_t" value="12345"/>             
	<parameter name="to" type="uint32_t" value="54483"/>
	<parameter name="nextHeader" type="uint8_t" value="NAN"/>
	<parameter name="reserved" type="uint8_t" value="NAN"/>
      </element>
    
      <function name="HelloMsg" direction="in"> 
	<parameter name="id" type="int" value="2" direction="in"/> 
	<parameter name="context" type="std::string" direction="out"/> 
      </function>
    
      <function name="HelloMsgAck" direction="out"> 
	<parameter name="id" type="int" value="2" direction="in"/> 
	<parameter name="context" type="std::string" direction="out"/> 
      </function>
      
    </package>


generic="true" *** Means that interface will use generic methods for communication,at each method will be pointer to GenericClient::Header


<element>  *** Additional Header parameters for IPCMessage are mandatory

<parameter name="" type="" value=""/> *** name of parameter, type and value required

<parameter name="msgCounter" type="uint8_t" value="m_counter"/> *** there it is implemented counter for count Messages

<parameter value="NAN"/> *** If |Value = NAN| -> parameter will not be initialized

<parameter name="timeStamp" type="uint32_t" value="getTimestamp()"/> *** there it is implemented timeStamp

<parameter name="from" /> *** ID of your application, have to be also like argument of script

<parameter name="to" />  *** ID of application with which you want communicate

    
<function name="HelloMsg" direction="in"> *** Name of function and direction are required |direction = in -> message will be received|

<function name="HelloMsgAck" direction="out"> *** Name of function and direction are required |direction = out -> message will be sent|



<parameter name="id" type="int" value="2" direction="in"/>  *** paramer name, type, value, direction required

<parameter direction="in"/> *** Direction in - parameter will not be atribute of function,  will be stored at payload /@name, just for internal information between interfaces

<parameter direction="out"/> *** Direction out - parameter will be atribute of function, will be stored at payloadl /payload/@name, for external usecase
    


