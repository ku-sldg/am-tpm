checkSuccess()
{
    if [ $1 -ne 0 ]; then
        echo " ERROR ERROR ERROR ERROR ERROR ERROR ERROR"
        exit 255
    fi
}

export PATH="$HOME/TPM_Simulator/tss160/utils:$PATH"
export LD_LIBRARY_PATH="$HOME/TPM_Simulator/tss160/utils"

#echo "Policy restart"
#policyrestart -ha 03000000 > run.out 
#checkSuccess $?

#echo "Flush the session"
#flushcontext -ha 03000000 > run.out
#checkSuccess $?

echo "Flush the platform primary key"
flushcontext -ha 80000000 > run.out
checkSuccess $?

echo "Flush the signing key"
flushcontext -ha 80000001 > run.out
checkSuccess $?