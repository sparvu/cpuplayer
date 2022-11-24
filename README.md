<img src="/docs/img/cpuplayer-logo.png" height="25%" width="25%" />

## Description

Performance Data Visualization of Multiprocessor Systems. cpuplayer is a CLI stand-alone version of [Barry2D](http://www.perfdynamics.com/Tools/tools.html#tth_sEc1) , that provides animation for visually exploring performance issues based on multiprocessor utilization data. Features:

- Stand-alone video capabilities to play real-time or historical utilization data

- Supports CSV data format

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

### Play historical data

This will play historical data from one data file, called cpurec.T5220.noprocsets

```
$ cpuplayer data/cpurec.T5220.noprocsets
```

### Play real-time data

This will run **cpurec** data recorder and pipe the results to cpuplayer directly to visualize data as soon as it is collected.

```
$ cpurec | cpuplayer 
```



## Examples

### Solaris 10 Sybase OLTP workload 

Case study which shows **cpuplayer** diplaying the performance advantages of enabling processor sets in Solaris 10 for a Sybase OLTP workload running on a Sun Entreprise T5220 with up to 32 virtual processors.


<p align="center">
  <img alt="No procsets" src="/docs/img/cpuplayer-noprocsets.gif" width="35%">
&nbsp; &nbsp;
  <img alt="procsets" src="/docs/img/cpuplayer-procsets.gif" width="35%">
</p>

## Future

Port cpuplayer to Rust. Pause option. 
