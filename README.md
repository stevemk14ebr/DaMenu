# DaMenu

*Work in Progress*

#C++ 11 Event Drive, Object Oriented GUI.

1. Renderer Backend end is DirectX 11 Provided by DirectX Toolkit Library.

2. Object Oriented To allow easy porting to different renderers and input methods

3. Events driven through callbacks
  * Internal virtual method callbacks for class to class events
  * std::function callbacks allow user to subscribe to class events
  * each class has std::function driven callbacks specific to that class
  * events use familiar += operator as subscription method
  
#Currently Supported Elements:
* Moveable Window

* Moveable Tabbed Window 

  * Tab Content Pages

* Button

* Slider

* Checkbox

![alt tag](http://i.imgur.com/ho2H8mA.png)
