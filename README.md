# build-push-gadget

A GitHub Action to build and push [Inspektor Gadget](https://inspektor-gadget.io/) gadget OCI images.

> **Prerequisite:** `ig` must be available on `PATH`. You can use [mqasimsarfraz/setup-ig](https://github.com/mqasimsarfraz/setup-ig) or install it however you prefer.

## Usage

```yaml
steps:
  - uses: actions/checkout@v6

  - uses: docker/login-action@v4
    with:
      registry: ghcr.io
      username: ${{ github.actor }}
      password: ${{ secrets.GITHUB_TOKEN }}

  - uses: mqasimsarfraz/setup-ig@v0

  - uses: mqasimsarfraz/build-push-gadget@v0
    with:
      gadget: my-gadget/
      image: ghcr.io/${{ github.repository }}/my-gadget:latest
```

### Build only (no push)

```yaml
  - uses: mqasimsarfraz/setup-ig@v0

  - uses: mqasimsarfraz/build-push-gadget@v0
    with:
      gadget: my-gadget/
      image: my-gadget:dev
      push: "false"
```

### Pin ig and builder versions

```yaml
  - uses: mqasimsarfraz/setup-ig@v0
    with:
      version: "v0.51.1"

  - uses: mqasimsarfraz/build-push-gadget@v0
    with:
      gadget: my-gadget/
      image: ghcr.io/${{ github.repository }}/my-gadget:latest
      builder-image: "ghcr.io/inspektor-gadget/gadget-builder:v0.51.1"
```

### Build, push and sign with cosign

To sign your gadget images, first generate a cosign key pair:

```bash
cosign generate-key-pair
```

This creates `cosign.key` (private) and `cosign.pub` (public). Store the private
key as `COSIGN_PRIVATE_KEY` and its password as `COSIGN_PASSWORD` in your
[repository secrets](https://docs.github.com/en/actions/security-for-github-actions/security-guides/using-secrets-in-github-actions).

Users can then verify your gadget with:
```bash
sudo ig run --public-keys="$(cat cosign.pub)" ghcr.io/myorg/mygadget:latest
```

See the [Inspektor Gadget signing](https://www.inspektor-gadget.io/docs/latest/gadget-devel/signing)
and [verification](https://www.inspektor-gadget.io/docs/latest/reference/verify-gadgets) docs for more details.

```yaml
  - uses: sigstore/cosign-installer@v4.1.1

  - uses: mqasimsarfraz/setup-ig@v0

  - uses: mqasimsarfraz/build-push-gadget@v0
    with:
      gadget: my-gadget/
      image: ghcr.io/${{ github.repository }}/my-gadget:latest
      cosign-private-key: ${{ secrets.COSIGN_PRIVATE_KEY }}
      cosign-password: ${{ secrets.COSIGN_PASSWORD }}
```

## Inputs

| Name               | Description                                            | Default                                                  | Required |
|--------------------|--------------------------------------------------------|----------------------------------------------------------|----------|
| `gadget`           | Path to the gadget source directory                    |                                                          | Yes      |
| `image`            | Full image reference (e.g. `ghcr.io/org/gadget:tag`)  |                                                          | Yes      |
| `builder-image`    | Builder image for compiling the gadget                 | `ghcr.io/inspektor-gadget/gadget-builder:latest`         | No       |
| `build-args`       | Additional arguments for `ig image build`              |                                                          | No       |
| `push`             | Whether to push the image after building               | `true`                                                   | No       |
| `cosign-private-key` | Cosign private key for signing (pass via `${{ secrets.* }}`) |                                                   | No       |
| `cosign-password`  | Password for the cosign private key (pass via `${{ secrets.* }}`) |                                              | No       |
