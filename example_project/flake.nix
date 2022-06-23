{
  description = "IceCream-Cpp example project";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";

    icecream-cpp = {
      url = "github:renatoGarcia/icecream-cpp";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = inputs@{ self, nixpkgs, ... }:
    let
      application = { pkgs }:
        pkgs.stdenv.mkDerivation {
          pname = "icecream-example";
          version = "1.0";
          src = ./.;

          nativeBuildInputs = with pkgs.buildPackages; [
            cmake
          ];

          buildInputs = with pkgs; [
            icecream-cpp
          ];
        };

      pkgs = (import nixpkgs {
        system = "x86_64-linux";
        overlays = [
          inputs.icecream-cpp.overlays.default
        ];
      });

    in {
      packages.x86_64-linux = {
        default = application { inherit pkgs; };
      };
    };
}
