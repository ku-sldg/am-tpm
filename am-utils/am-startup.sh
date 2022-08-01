checkSuccess()
{
    if [ $1 -ne 0 ]; then
        echo " ERROR ERROR ERROR ERROR ERROR ERROR ERROR"
        exit 255
    fi
}

export PATH="$HOME/TPM_Simulator/tss160/utils:$PATH"
export LD_LIBRARY_PATH="$HOME/TPM_Simulator/tss160/utils"


echo "[TPM] Power cycle and startup"
powerup > run.out
checkSuccess $?
startup -c > run.out
checkSuccess $?

echo "[TPM] Create a platform primary RSA storage key"
createprimary -hi p -pwdk sarahStoragePrimary -pol $HOME/TPM_Simulator/tss160/utils/policies/zerosha256.bin -tk ticket.bin -ch creationHash.bin > run.out
checkSuccess $?

echo "[TPM] Create an RSA 2048-bit sha512 unrestricted signing key under the primary key"
create -hp 80000000 -rsa 2048 -halg sha512 -si -kt f -kt p -opr priv.bin -opu pub.bin -opem pub.pem -pwdp sarahStoragePrimary -pwdk sarahSign > run.out
checkSuccess $?

echo "[TPM] Load the RSA 2048-bit decryption key under the primary key"
load -hp 80000000 -ipr priv.bin -ipu pub.bin -pwdp sarahStoragePrimary > run.out
checkSuccess $?


