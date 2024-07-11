# Acknowledgement
Author: Andrew Tabaczynski

This project was devleoped as part of a larger internship project at Oak Ridge National Labratory and is being released for use by the public. 

# Overview
This is a plugin for the Saleae logic-2 software for easy-to-use analysis of the Wiegand Protocol and allows for different size site, facility, and card numbers, which the program also partitions. Data is displayed in an easy-to-read form, and also allows for multiple signals within the same measurement period. Below, is an example of the plug-in being used, first we start with the card which is being scanned. 

<img src="https://github.com/AndrewTabs1038/Wiegand-Analyzer/assets/135442448/c44f9ecd-8c97-43dc-a9e9-369a1a65fe84" width="750" height="400"/>

Notice the facility code 084, and the user number 31066. This badge is scanned, and the communication channels between the badge reader and controller are captured within a logic analyzer. Then, doing some preliminary investigation of the signals being transmitted, it's discovered that the site length is 0, the facility is 16, and the user is 16. By entering this information within the analyzer, we can then easily read the transmitted data. Here, you can see that we uncovered the facility code and user number.

<img src="https://github.com/AndrewTabs1038/Wiegand-Analyzer/assets/135442448/0a71d118-0143-4e7d-8a82-0d9ad88de297" width="1500" height="250"/>

# To Run
In order to take advantage of this plug-in, you need to use a saleae logic analyzer as well as the [supporting software](https://www.saleae.com/pages/downloads). You will also need a C++ compiler; it is recommended to have Visual Studio 2022 (or Visual Studio 2017 and later).

After downloading this repo, copy the Debug directory as discussed [here](https://support.saleae.com/faq/technical-faq/setting-up-developer-directory). Note, that having the src code is not required to use this plugin, just the files within the debug folder. 

Once this is done, you should see the Wiegand analyzer. Next, set your parameters, the default is set up for the standard 26-bit Wiegand:


<img src="https://github.com/AndrewTabs1038/Wiegand-Analyzer/assets/135442448/c51688af-75fe-485c-93d5-9fd1079294b2" width="300" height="300"/>

<img src="https://github.com/AndrewTabs1038/Wiegand-Analyzer/assets/135442448/6c133373-4e96-4fc4-87bd-f01424cac10b" width="300" height="300"/>
