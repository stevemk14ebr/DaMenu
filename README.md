# DaMenu

*Work in Progress*

#C++ 11 Event Drive, Object Oriented GUI.

1. Renderer Backend end is DirectX 11 Provided by DirectX Toolkit Library.

2. Object Oriented To allow easy porting to different renderers and input methods

3. Events driven through callbacks
  * Internal virtual method callbacks provide internal super class to derived class events
  * std::function based EventDispatch callbacks allow user to subscribe to class events
  * EventDispatcher callbacks provide internal derived class to super class communication
  * each class has EventDispatcher callbacks specific to that class
  * EventDispatcher uses familiar += operator as subscription method
  
#Currently Supported Elements:
* Moveable Window

* Moveable Tabbed Window 

  * Tab Content Pages

* Button

* Slider

* Checkbox

![alt tag](http://i.imgur.com/xwPnt7U.gif)
