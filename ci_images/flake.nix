{
  description = "Images to IceCreamCpp CI testing";

  inputs = {
    nixpkgs_24_11.url = "nixpkgs/nixos-24.11";
    nixpkgs_24_05.url = "nixpkgs/nixos-24.05";
    nixpkgs_23_11.url = "nixpkgs/nixos-23.11";
  };

  outputs = { self, nixpkgs_24_11, nixpkgs_24_05, nixpkgs_23_11 }:
  let
    lib = nixpkgs_24_11.lib;

    ci-docker = { pkgs, stdenv, tagname }:
      pkgs.dockerTools.buildLayeredImage {
        name = "renatogarcia/icecream-ci";
        tag = tagname;

        contents = with pkgs; [
          busybox
          cmake
          conan
          gnumake
          stdenv.cc
        ];

        # Required by clang using mktemp and failing when /tmp doesn't exist.
        extraCommands = ''
            mkdir -p tmp
        '';

        config = {
          WorkingDir = "/home";
          Env =
             # Required so that the linker can find the libc++ when compiling with clang
            lib.lists.optionals (stdenv.cc.libcxx != null) [
              "NIX_CC_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu=1"
              (
                let
                  # clang8 depends on libc++abi besides libc++
                  findLibcxxabi = depsList: lib.lists.findFirst (drv: drv.pname == "libcxxabi") null depsList;
                  libcxxabi = findLibcxxabi (lib.lists.remove null stdenv.cc.libcxx.buildInputs);
                in
                  if libcxxabi != null
                  then "NIX_LDFLAGS=-L${stdenv.cc.libcxx}/lib/ -L${libcxxabi.out}/lib"
                  else "NIX_LDFLAGS=-L${stdenv.cc.libcxx}/lib/"
              )
            ];
        };
      };

    pkgs_24_11 = import nixpkgs_24_11 {
      system = "x86_64-linux";
    };

    pkgs_24_05 = import nixpkgs_24_05 {
      system = "x86_64-linux";
    };

    pkgs_23_11 = import nixpkgs_23_11 {
      system = "x86_64-linux";
    };

  in
  {
    packages.x86_64-linux = {
      ci-gcc6 = ci-docker { pkgs = pkgs_24_11; stdenv = pkgs_24_05.gcc6Stdenv; tagname = "gcc6-3"; };
      ci-gcc9 = ci-docker { pkgs = pkgs_24_11; stdenv = pkgs_24_11.gcc9Stdenv; tagname = "gcc9-3"; };
      ci-gcc14 = ci-docker { pkgs = pkgs_24_11; stdenv = pkgs_24_11.gcc14Stdenv; tagname = "gcc14-3"; };
      ci-clang8 = ci-docker { pkgs = pkgs_24_11; stdenv = pkgs_23_11.llvmPackages_8.libcxxStdenv; tagname = "clang8-3"; };
      ci-clang19 = ci-docker { pkgs = pkgs_24_11; stdenv = pkgs_24_11.llvmPackages_19.libcxxStdenv; tagname = "clang19-3"; };
    };
  };
}
