# secret-santa
Secret santa program in C

## Usage

```bash
gcc -o secret_santa main.c
echo "albert bobby colin" | ./secret_santa

Seed: 12345
colin: <i>(base 64 encoding of the word colin, so that you can copy this output to colin without accidentally knowing who the receipient is)</i>
albert: <i>(base 64 encoding of the word bobby, so that you can copy this output to albert without accidentally knowing who the receipient is)</i>
bobby: <i>(base 64 encoding of the word albert, so that you can copy this output to bobby without accidentally knowing who the receipient is)</i>
```

### Arguments
`-s` seed, for replicability.
`-d` decode/debug, leave the recipients as plaintext.

