####################################################################################################
#
####################################################################################################


####################################################################################################
#Directories for library files 
####################################################################################################
vob_lib_dirs :=

####################################################################################################
#library files
####################################################################################################
vob_lib_files :=

####################################################################################################
#Directories for include files
####################################################################################################

####################################################################################################
#Directories for source files
####################################################################################################
vob_src_dirs := $(CODEC_SRC_FILE_PATH)/om/src

####################################################################################################
#Source files
####################################################################################################
vob_src_files :=$(vob_src_dirs)/om_comm.c        \
	$(vob_src_dirs)/../../../common/src/om/om_cpuview.c     \
	$(vob_src_dirs)/om_cpuload.c     \
	$(vob_src_dirs)/../../../common/src/om/om_log.c         \
	$(vob_src_dirs)/om_msghook.c     \
	

####################################################################################################
#general make rules
####################################################################################################


####################################################################################################
#
####################################################################################################


####################################################################################################
#
####################################################################################################