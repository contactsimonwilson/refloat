# Refloat for ADV2
Self-balancing skateboard package based on the Float package by Mitch Lustig, Dado Mista and Nico Aleman: Float, Refactored.

This special version for ADV2 is based on the Refloat preview-v1.1 branch and includes:
- **BMS tiltback support for the ADV2**
- **Haptic buzz, enabled by default**

Refloat 1.0 adds the following main features:
- **Separate Axis KP**
- **Brand new full-featured GUI in VESC Tool**
- **Advanced LED lighting control for LEDs connected directly to VESC**

Refloat configuration and tunes are 100% compatible with Float 1.3 (and hence also 2.0, apart from missing a few newly added options). Refloat also maintains compatibility with 3rd party apps (Floaty / Float Control), though it works best with its brand new VESC Tool interface.

#### Separate Axis KP
The Mahony KP can now be configured separately for each IMU axis. This feature improves the way the board handles in turns and in a significant way improves the balance profile.

Read the descriptions of the new configuration options for more information.

### First Installation
Backup your configuration by exporting the XML or pressing the BACKUP button, install Refloat and then either Restore or load the backup file. 

#### Tune Quicksaves
Tune quicksaves will be converted from Float to Refloat on first start. Your Float tunes won't be touched and if you go back to Float, you will find them unchanged.

IMU quicksaves were removed from Refloat.

#### Tune Archive
Refloat uses the same Tune archive as Float, the same tunes are available.

### Disclaimer
Use at your own risk. Electric vehicles are inherently dangerous, authors of this package shall not be liable for any damage or harm caused by errors in the software. Not endorsed by the VESC project.

### Credits
Author: Lukáš Hrázký

Original Float package authors: Mitch Lustig, Dado Mista, Nico Aleman

### Download and Changelog
[https://github.com/lukash/refloat/releases](https://github.com/lukash/refloat/releases)
