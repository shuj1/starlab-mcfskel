# Retrieve environment variables
MATLAB = $$(MATLAB)
DYLD_LIBRARY_PATH = $$(DYLD_LIBRARY_PATH)


isEmpty(MATLAB){
    warning("MATLAB should point to matlab.app folder, defaulting it to /Applications/matlab.app")
    MATLAB = /Applications/matlab.app
}

macx{
    isEmpty(DYLD_LIBRARY_PATH){
        warning("DYLD_LIBRARY_PATH should point to $$MATLAB/bin/maci64")
        warning("defaulting it to $$MATLAB/bin/maci64")
        warning("NOTE: application might compile but crash on execution!!!")
        DYLD_LIBRARY_PATH = "$$MATLAB/bin/maci64"
    }
    LIBS += -L$$DYLD_LIBRARY_PATH
    LIBS += -leng -lmex -lmat -lmx -lut
}

win32{
    MATLAB_LIBS = "$$MATLABextern\\lib\\win32\\microsoft"
    LIBS += $$MATLAB_LIBS\\libeng.lib $$MATLAB_LIBS\\libmex.lib \
            $$MATLAB_LIBS\\libmat.lib $$MATLAB_LIBS\\libmx.lib $$MATLAB_LIBS\\libut.lib
}

INCLUDEPATH += $$(MATLAB)/extern/include/
