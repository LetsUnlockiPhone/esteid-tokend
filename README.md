# EstEID.tokend

 * License: LGPL 2.1
 * &copy; Estonian Information System Authority
 * For OSX 10.9 to 10.12

### Background

For general reading on Tokend history, please see [OpenSC.tokend](https://github.com/OpenSC/OpenSC.tokend#history-of-tokend-support-in-os-x). It also describes the build-time dependency on binary-only components necessary to build a tokend, distributed in the build folder.

## Building
[![Build Status](https://travis-ci.org/open-eid/esteid-tokend.svg?branch=master)](https://travis-ci.org/open-eid/esteid-tokend)


### OpenSSL
OpenSSL has been deprecated as a system API since OSX 10.7 (July 2011) and since Xcode 7 (September 2015) OpenSSL headers have been removed from the SDK-s. The version of OpenSSL available on OSX has always been pre-historic.

Current codebase depends on OpenSSL for certificate parsing and some other tasks, thus it needs to be included statically, until refactoring has been made to not depend on OpenSSL.

## Instructions
- Fetch the source

        git clone --recursive https://github.com/open-eid/esteid-tokend
        cd esteid-tokend

- Fetch and compile a static version of OpenSSL

        make ossl

- Build & make an installable package

        make package

- To build a [signed version](https://developer.apple.com/developer-id/), specify SIGNCERT and INSTCERT on the command line

        make signedpackage SIGNCERT="Mac Developer" INSTCERT="Distribution signer"

## Debugging

```
touch /tmp/esteid-tokend.log
chmod 766 /tmp/esteid-tokend.log
```

## Support
Official builds are provided through official distribution point [installer.id.ee](https://installer.id.ee). If you want support, you need to be using official builds. Contact for assistance by email [abi@id.ee](mailto:abi@id.ee) or [www.id.ee](http://www.id.ee).

Source code is provided on "as is" terms with no warranty (see license for more information). Do not file Github issues with generic support requests.
