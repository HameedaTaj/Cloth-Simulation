#!/usr/bin/python


import os
import sys
import pba.swig.PbaViewer as pbav
import pba.swig.PbaThings as things

viewer = pbav.CreateViewer()

args = pbav.StringArray()

for s in sys.argv:
	args.push_back(s)


particle = things.ClothSim()
viewer.AddThing(particle)
viewer.Init(args)

viewer.MainLoop()

