# secret-santa
Simple secret santa program in C, outputting a base64-encoded list of recievers that can be pasted to the corresponding givers.

## Usage

```bash
> gcc -o secret_santa main.c
> echo "albert bobby colin" | ./secret_santa
Seed: 12345
colin: (base 64 encoding of the word bobby)
albert: (base 64 encoding of the word colin)
bobby: (base 64 encoding of the word albert)
```

### Arguments
`-s` seed, for replicability.
`-d` decode/debug, leave the recipients as plaintext.

