#this is not mean to be run locally
#
echo "ENV..................................."
env 
echo "VOMS"
voms-proxy-info -all
echo "CMSSW BASE, python path, pwd"
echo $CMSSW_BASE 
echo $PYTHON_PATH
echo $PWD 
echo "Adding current directory to PYTHONPATH"
export PYTHONPATH=$PYTHONPATH:$PWD

echo Found Proxy in: $X509_USER_PROXY
JOBID=$1
shift

echo "JobID: $JOBID"
echo "All remaining arguments:"
echo "$@"

python3 Lobster_postproc.py "$@"
echo "<FrameworkJobReport></FrameworkJobReport>" > FrameworkJobReport.xml
