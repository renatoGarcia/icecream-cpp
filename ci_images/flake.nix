{
  description = "Images to IceCreamCpp CI testing";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-23.11";
  };

  outputs = { self, nixpkgs }:
  let
    ci-docker = { pkgs, compiler, tagname }:
      pkgs.dockerTools.buildLayeredImage {
        name = "icecream-ci";
        tag = tagname;

        contents = with pkgs; [
          compiler
          gnumake
          cmake
          boost.dev
          busybox
        ];

        # Required by clang using mktemp and failing when /tmp doesn't exist.
        extraCommands = ''
            mkdir -p tmp
        '';

        config = {
          WorkingDir = "/home";
          Env = [
            "Boost_INCLUDE_DIR=/include"
          ];
        };
      };

    pkgs = import nixpkgs {
      system = "x86_64-linux";
    };

  in
  {
    packages.x86_64-linux = {
      ci-gcc13 = ci-docker { pkgs = pkgs; compiler = pkgs.gcc13; tagname = "gcc13"; };
      ci-clang17 = ci-docker { pkgs = pkgs; compiler = pkgs.clang_17; tagname = "clang17"; };
    };
  };
}
