#include "myHostInfo.h"
#include "myLog.h"
#include "myException.h"

myHostInfo::myHostInfo()
{

	openHostDb();

}

myHostInfo::myHostInfo(string& hostName,hostType type)
{
	
	searchHostDB = 0;
	

	try 
	{
		if (type == NAME)
		{
			// Retrieve host by name
			hostPtr = gethostbyname(hostName.c_str());

			if (hostPtr == NULL)
			{
				
					myException gethostbynameException(0,"unix: error getting host by name");
					throw gethostbynameException;
				
			}
        }
		else if (type == ADDRESS)
		{
			// Retrieve host by address
		    unsigned long netAddr = inet_addr(hostName.c_str());
			if (netAddr == -1)
			{
				myException inet_addrException(0,"Error calling inet_addr()");
				throw inet_addrException;
			}

	        hostPtr = gethostbyaddr((char *)&netAddr, sizeof(netAddr), AF_INET);
			if (hostPtr == NULL)
			{
				
					myException gethostbynameException(0,"unix: error getting host by name");
					throw gethostbynameException;
				
			}
        }
		else
		{
			myException unknownTypeException(0,"unknown host type: host name/address has to be given ");
			throw unknownTypeException;
		}
    }
	catch(myException& excp)
	{
		excp.response();
		exit(1);
	}
}



char myHostInfo::getNextHost()
{
	// winLog<<"UNIX getNextHost() is called...\n";
    // Get the next host from the database
    if (searchHostDB == 1)
    {
         if ((hostPtr = gethostent()) == NULL)
              return 0;
         else
              return 1;
    }
    return 0;
}


