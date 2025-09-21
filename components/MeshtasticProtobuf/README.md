ESP-IDF component for the Meshtastic protobuf generated C files.

I had issues with standalone-ui and device-ui expecting slightly different versions, since this 
repo is tracking trunks of the mainline repos, so I decided to generate our own fresh code from 
protobufs.

Update submodules. 

Make sure that `nanopb`'s directory `bin/` is in your path, for example:

```shell
export PATH=$PATH:~/Downloads/nanopb-0.4.9.*/generator-bin
```

and then run `./regenerate_protobufs.sh`.