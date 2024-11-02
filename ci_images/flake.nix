{
  description = "Images to IceCreamCpp CI testing";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    nixpkgs_24_05.url = "nixpkgs/nixos-24.05";
    nixpkgs_23_11.url = "nixpkgs/nixos-23.11";
  };

  outputs = { self, nixpkgs, nixpkgs_24_05, nixpkgs_23_11 }:
  let
    ci-docker = { pkgs, compiler, tagname }:
      pkgs.dockerTools.buildLayeredImage {
        name = "renatogarcia/icecream-ci";
        tag = tagname;

        contents = with pkgs; [
          busybox
          cmake
          compiler
          conan
          gnumake
        ];

        # Required by clang using mktemp and failing when /tmp doesn't exist.
        extraCommands = ''
            mkdir -p tmp
        '';

        config = {
          WorkingDir = "/home";
        };
      };

    pkgs = import nixpkgs {
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
      ci-gcc6 = ci-docker { pkgs = pkgs; compiler = pkgs_24_05.gcc6; tagname = "gcc6"; };
      ci-gcc9 = ci-docker { pkgs = pkgs; compiler = pkgs.gcc9; tagname = "gcc9"; };
      ci-gcc14 = ci-docker { pkgs = pkgs; compiler = pkgs.gcc14; tagname = "gcc14"; };
      ci-clang8 = ci-docker { pkgs = pkgs; compiler = pkgs_23_11.clang_8; tagname = "clang8"; };
      ci-clang18 = ci-docker { pkgs = pkgs; compiler = pkgs.clang_18; tagname = "clang18"; };
    };
  };
}
