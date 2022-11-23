# cpuplayer

## Description

Performance Data Visualization of Multiprocessor Systems. cpuplayer is a CLI stand-alone version of [Barry2D](http://www.perfdynamics.com/Tools/tools.html#tth_sEc1) , that provides animation for visually exploring performance issues based on multiprocessor utilization data. Features:

- Stand-alone video playback of historical multiprocessor utilization data

- Data format can be as simple as CSV

- Toggled display of date and time information

- Toggled display of multicore IDs

- Toggled display of USR, SYS and IDL axes

- Full color performance zones

<div align="center">
<img src="/docs/img/cpuplayer.gif" height="40%" width="40%" />
</div>

## Build

The cpuplayer package comes in source code, C. To compile this for your system please execute the followings:

  1. export OSTYPE=SunOS or Darwin or Linux depends on your system
    
  2. make clean
    
  3. make
     
  4. execute the program: cpuplayer data/1cpu.ferrari4000

## Usage

<div align="center">
<img src="/docs/img/cpuplayer_help.png" height="50%" width="50%" />
</div>
