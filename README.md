# cpuplayer

## Description

Performance Data Visualization of Multiprocessor Systems. cpuplayer is a CLI stand-alone version of [Barry2D](http://www.perfdynamics.com/Tools/tools.html#tth_sEc1) , that provides animation for visually exploring performance issues based on multiprocessor utilization data. Features:

- Stand-alone video playback of historical or real-time multiprocessor utilization data

- CSV data format

- Toggled display of date and time information

- Toggled display of multicore IDs

- Toggled display of USER, SYSTEM and IDLE axes

- Full color performance zones

<div align="center">
<img src="/docs/img/cpuplayer.gif" height="40%" width="40%" />
</div>

## Build

The cpuplayer package comes in source code, C. To compile this for your system please execute the followings:

  1. export OSTYPE=Darwin | Linux 
    
  2. make clean
    
  3. make
     
  4. execute the program: cpuplayer data/1cpu.ferrari4000

## Usage

<div align="center">
<img src="/docs/img/cpuplayer_help.png" height="50%" width="50%" />
</div>


## Use cases

This shows the videos from one case study which shows CPUplayer diplaying the performance advantages of enabling processor sets in Solaris 10 for a Sybase OLTP workload running on a Sun Entreprise T5220 with up to 32 virtual processors.

