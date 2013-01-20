# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# For each target create a dummy rule so the target does not have to exist
/opt/local/lib/libtbb.dylib:
/opt/local/lib/libtbbmalloc.dylib:


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.LandySzalayEstimator.Debug:
/Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/Debug/LandySzalayEstimator:\
	/opt/local/lib/libtbb.dylib\
	/opt/local/lib/libtbbmalloc.dylib
	/bin/rm -f /Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/Debug/LandySzalayEstimator


PostBuild.LandySzalayEstimator.Release:
/Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/Release/LandySzalayEstimator:\
	/opt/local/lib/libtbb.dylib\
	/opt/local/lib/libtbbmalloc.dylib
	/bin/rm -f /Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/Release/LandySzalayEstimator


PostBuild.LandySzalayEstimator.MinSizeRel:
/Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/MinSizeRel/LandySzalayEstimator:\
	/opt/local/lib/libtbb.dylib\
	/opt/local/lib/libtbbmalloc.dylib
	/bin/rm -f /Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/MinSizeRel/LandySzalayEstimator


PostBuild.LandySzalayEstimator.RelWithDebInfo:
/Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/RelWithDebInfo/LandySzalayEstimator:\
	/opt/local/lib/libtbb.dylib\
	/opt/local/lib/libtbbmalloc.dylib
	/bin/rm -f /Users/Pamilator/Work/mp-epitech/project-base/ParallelComp_Project/bin/RelWithDebInfo/LandySzalayEstimator


