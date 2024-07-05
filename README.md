# Acknowledgement
Author: Andrew Tabaczynski

This project was devleoped as part of a larger internship project at Oak Ridge National Labratory and is being released for use by the public. 

# Overview
This is a plugin for the Salea logic-2 software for easy-to-use analysis of the Wiegand Protocol and allows for different size site, facility, and card ID numbers, which the program also partitions. Data is displayed in a easy-to-read form, and also allows for multiple signals within the same time frame. Below, is an example of the plug-in being used, first we start with the card that is being scanned in the example. 

![image](https://github.com/AndrewTabs1038/Wiegand-Analyzer/assets/135442448/c44f9ecd-8c97-43dc-a9e9-369a1a65fe84)

Notice the facility number 084, and the user ID number 31066. This badge is scanned, and the communicaiton channels between the badge reader and contorler are captured within a logic analyizer. Then, doing some preliminary investigation of the signals being transmitted, it's discovered that the site length is 0, the facility us 16, and the user ID length is 16. By entering this infromation within the analyizer, we can then easily read the transmitted data. Here, you can see that we uncovered the facility and user ID numbers. 

![image](https://github.com/AndrewTabs1038/Wiegand-Analyzer/assets/135442448/0a71d118-0143-4e7d-8a82-0d9ad88de297)

# To Run
In order to take advantage of this plug-in, you need to use a salea logic analyizer as well as the [supporting software](https://www.saleae.com/pages/downloads). You will also need a C++ compiler, it is recommended to have Visual Studio 2022 (or Visual Studio 2017 and later).
