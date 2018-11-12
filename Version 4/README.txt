This is my updated version.

You may have to edit logging.cpp line 123, in order to get it working properly on your computer

I wrote this version to make the program easier to read and less intensive on the thread making the log
This puts more work on the thread processing the logs. 
There is also no using namespace std to avoid issues that can cause in the future with other systems. 

The process is up and running the way it should be. There does not seem to be any errors in any of the functions thankfully. 
CreateDirectory is giving me some serious hell. Not sure why its casuing problems but they seem to just diapper sometimes... It appears to be more directed to the ide and how it handles unicode. I found a workaround to be to simply close and reopen the ide if the ide is visual studio. 
