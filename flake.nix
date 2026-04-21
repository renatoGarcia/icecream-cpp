{
  description = "A little library to help with print debugging in C++11 and later";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    systems = {
      url = "github:nix-systems/default";
      flake = false;
    };
  };

  outputs =
    inputs@{
      self,
      nixpkgs,
      flake-parts,
      ...
    }:
    let
      cmakeContent = builtins.readFile ./CMakeLists.txt;

      versionNumbersRegex = ".*project[[:space:]]*\\(.*[[:space:]]VERSION[[:space:]]+([[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+).*\\).*";

      versionNumbers = builtins.head (builtins.match versionNumbersRegex cmakeContent);

      devHash_ = builtins.match ".*DEV_HASH: ([[:xdigit:]]{8}).*" cmakeContent;
      devHash = if devHash_ != null then builtins.head devHash_ else null;

      version = versionNumbers + (if devHash != null then "+dev." + devHash else "+local_repo");

      library =
        { pkgs }:
        pkgs.stdenv.mkDerivation {
          pname = "icecream-cpp";
          inherit version;

          src = ./.;

          nativeBuildInputs = [ pkgs.buildPackages.cmake ];

          meta = with pkgs.lib; {
            homepage = "https://github.com/renatoGarcia/icecream-cpp";
            description = "A little library to help with print debugging in C++11 and later";
            license = licenses.mit;
            maintainers = with maintainers; [ renatoGarcia ];
          };
        };

      dev-env =
        { pkgs, stdenv }:
        let
          package = library { inherit pkgs; };
        in
        pkgs.mkShell.override { inherit stdenv; } {
          name = "icecream-cpp";
          nativeBuildInputs =
            with pkgs.buildPackages;
            [
              (fmt.override { inherit stdenv; })
              boost
              clang-tools
              cmakeCurses
              conan
              range-v3
            ]
            ++ package.nativeBuildInputs;
          hardeningDisable = [ "fortify" ];
        };

    in
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = import inputs.systems;

      flake = {
        overlays.default = final: prev: {
          icecream-cpp = library { pkgs = final; };
        };
      };

      perSystem =
        { system, ... }:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ self.overlays.default ];
          };
        in
        {
          packages.default = pkgs.icecream-cpp;

          devShells = rec {
            gcc = dev-env {
              inherit pkgs;
              stdenv = pkgs.stdenv;
            };
            clang = dev-env {
              inherit pkgs;
              stdenv = pkgs.libcxxStdenv;
            };
            default = gcc;
          };

          formatter = pkgs.nixfmt-rfc-style;
        };
    };
}
