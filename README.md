# dosbox-redpill v0.1

### Platform
linux x86_64

### Download
```bash
git clone https://github.com/mathfigure/dosbox
```

### Build
```bash
cd dosbox
./autogen.sh
./configure LIBS=-lrt
make
```

### Install (optional)
```bash
sudo make install
```

### Config
```bash
# run once and exit
src/dosbox
>exit

# find 'memsize' and change it
leafpad ~/.dosbox/dosbox-SVN.conf
>memsize=1
```

### Run
```bash
dosbox
```

### Test
```bash
hexedit /dev/shm/DOSBOX_RAM
ffplay /dev/shm/DOSBOX_RAM -f rawvideo -video_size 1024x1024 -pixel_format gray -framerate 25 -loop 0
gst-launch-1.0 multifilesrc location=/dev/shm/DOSBOX_RAM ! videoparse width=1024 height=1024 framerate=25 format=25 ! videoconvert dither=0 ! videoscale method=0 ! ximagesink
```
