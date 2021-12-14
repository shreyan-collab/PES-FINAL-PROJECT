# PES-FINAL-PROJECT

Github URL:
https://github.com/shreyan-collab/PES-FINAL-PROJECT.git

Video URL: https://drive.google.com/drive/folders/1OXiwXq0vsnInMMWZApwPhWgTRiT4M86i?usp=sharing

If you cannot view the file, try to download
If you are getting the following error, enable third party cookies for google drive

Try these steps

On your computer, open Chrome.
At the top right, click More. Settings.
Click Privacy and security. Cookies and other site data.
Select an option: Allow all cookies. Block all cookies (not recommended). Block third-party cookies in Incognito. Block third-party cookies.
 
My final project is to measure a digital gauge meter.


Features of digital gauge ( specifics not mentioned in the proposal):

1) The digital gauge meter can measure angles from 0 -180 degrees. 
2) The user is also having a provision to software calibration of zero references from 0 - 90 degrees with respect to which he can calculate the angle.
3) I am using the rolling reading to measure the angle from the accelerometer


In our application, the user can calibrate or have reference zero only from 0 - 90 degrees beyond the user is not having a provision to give zero references because the angle with which he can measure will become less as we can measure only from 0 to 180 degree. This is the reason we are giving zero references from 0 - 90 degrees.

The project consists of the following segments

1)Testing 
2)Command processor
Calibrate  - to calibrate a zero reference or actual zero as the starting point for measuring the angle
Set an angle - To measure an angle from the calibrated offset
Using PWM to indicate how close or far the user is from the destination/ final angle
Through roll command, the user can get the current roll value
Through the info command, the user can see all the possible commands and their syntax
Through author command, the user
The command processor also handles backspace, case insensitiveness, wrong syntax


Additional feature implemented

I have implemented an additional feature that will the user visual feedback on how close or how far he is from the destination angle.  I have not included it in the proposal we submitted earlier. As the user is far away from the destination angle, it transitions from GREEN to BLUE. As the user is close to the destination angle, it transitions from GREEN to RED.

All the features which have been mentioned in the proposal have been implemented.






