# HourlyFortune

A simple c++ app to show fortunes and create a replicant on the desktop.
By default it looks for fortunes at /boot/system/non-packaged/data/fortunes. But if there's no fortunes there, or the folder doesn't exist, it falls back to the default system fortunes folder in /boot/system/data/fortunes.
Just for test cases you can create a file named "custompath" in the executable folder, in this file you can write the path to your custom fortunes folder. Anyway this folder cannot be used by the replicant. So if your fortunes folder is ok you can copy/move its files in /boot/system/non-packaged/data/fortunes folder.
