checkSuccess()
{
    if [ $1 -ne 0 ]; then
        echo " ERROR ERROR ERROR ERROR ERROR ERROR ERROR"
        exit 255
    fi
}

UTILSPATH="$HOME/TPM_Simulator/tss160/utils"

export PATH="$UTILSPATH:$PATH"
export LD_LIBRARY_PATH="$UTILSPATH"
#export TPM_SESSION_ENCKEY
#export TPM_ENCRYPT_SESSIONS=0

echo "[TPM] Power cycle and startup"
powerup > run.out
checkSuccess $?
startup -c > run.out
checkSuccess $?

#TPM_SESSION_ENCKEY=`getrandom -by 16 -ns`

echo "[TPM] Create a platform primary RSA storage key"
# Password
#createprimary -hi p -pwdk sarahStoragePrimary -pol $HOME/TPM_Simulator/tss160/utils/policies/zerosha256.bin -tk ticket.bin -ch creationHash.bin > run.out
# Policy
#createprimary -hi p -pol $UTILSPATH/policies/zerosha256.bin -tk ticket.bin -ch creationHash.bin -uwa > run.out
# No auth
createprimary -hi p > run.out
checkSuccess $?

#echo "[TPM] Start a policy session"
# Policy
#startauthsession -se p > run.out
#checkSuccess $?

echo "[TPM] Create and load an RSA 2048-bit sha512 unrestricted signing key under the primary key"
# Policy with parent password
#create -hp 80000000 -rsa 2048 -halg sha512 -si -kt f -kt p -opr privS-encSRK.bin -opu pubS.bin -pwdp sarahStoragePrimary -pol policies/policyccsign.bin -uwa > run.out
# Policy with parent policy
#create -hp 80000000 -rsa 2048 -halg sha512 -si -kt f -kt p -opr privS-wrapped.bin -opu pubS.bin -opem pubS.pem -pol $UTILSPATH/policies/policyccsign.bin -uwa -se0 03000000 1 > run.out
# No auth with parent no auth
create -hp 80000000 -rsa 2048 -halg sha512 -si -kt f -kt p -opr priv.bin -opu pub.bin -opem pub.pem > run.out
checkSuccess $?

# Policy with parent password
#load -hp 80000000 -ipr privS-encSRK.bin -ipu pubS.bin -pwdp sarahStoragePrimary > run.out
# Plicy with parent policy
#load -hp 80000000 -ipr privS-wrapped.bin -ipu pubS.bin -se0 03000000 1
# No auth with parent no auth
load -hp 80000000 -ipr priv.bin -ipu pub.bin > run.out
checkSuccess $?



#echo "[TPM] Policy command code - sign"
# Policy
#policycommandcode -ha 03000000 -cc 15d > run.out
#checkSuccess $?

echo "Sign"
# Policy
sign -hk 80000001 -halg sha512 -salg rsa -if msg1 -os sig.bin -se0 03000000 1 > run.out
# No auth
sign -hk 80000001 -halg sha512 -salg rsa -if msg1 -os sig.bin > run.out
checkSuccess $?



