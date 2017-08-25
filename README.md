Installation
------------
You may need to change the port by hand if your USB-TTL adapter comes up as
something different than /dev/ttyUSB0 (*hint*Windows COM ports*hint*).

    virtualenv .
    . bin/activate
    # didn't work for me
    #pip install pygame --allow-external pygame --allow-unverified pygame
    # worked
    pip install hg+http://bitbucket.org/pygame/pygame


Usage
-----

    python here.py /dev/ttyUSB0

Now type in the black window, every keystroke should be mapped and forwarded,
modifier keys like ctrl, alt and the window key should be working.

Links
-----
For a video showing the the system in use, you can go to:
https://www.youtube.com/watch?v=T3_Bwmw8XRw
