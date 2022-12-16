<img src="/docs/img/cpuplayer-title4.png" />

## Description

Performance Data Visualization of Multiprocessor Systems. cpuplayer is a CLI stand-alone version of [Barry2D](http://www.perfdynamics.com/Tools/tools.html) , that provides animation for visually exploring performance issues based on multiprocessor utilization data. Features:

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

## Input Data

The cpuplayer expects the input data via command line arguments or directly piped to it, using a simple CSV format:

- timestamp: UNIX time
- cpuid: the CPU id
- User cpu percent utilization 0-100
- System cpu percent utilization 0-100
- Idle state cpu percent utilization 0-100

```
1263821686:3:8:47:45
1263821686:17:7:43:49
1263821686:2:9:44:47
1263821686:1:8:47:45
1263821686:18:8:42:50
1263821686:0:10:45:45
1263821686:16:7:45:48
1263821686:19:7:44:49
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
