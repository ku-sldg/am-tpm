# About
Adaption of IBM's TSS by Ken Goldman for the purpose of providing TPM functionality to the Copland Attestation Manager in CakeML.

Note: Not yet integrated with the AM.

# Building
Navigate to `am-utils` and type `make -f makefileTssFFI`.

# Testing
Startup your software TPM. Run the setup script with `bash am-startup.sh`. Then type `./TssSignFFI.cake` followed by `./TssVerifysignatureFFI.cake`.
