{
  description = "JUCE flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    juce-src = {
      url = "github:juce-framework/JUCE/8.0.8";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, juce-src }:
    let
      # Support multiple systems
      supportedSystems = [ "x86_64-linux" ];

      # Helper function to generate outputs for each system
      forEachSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs { inherit system; };
      });
    in
    {
      # JUCE package
      packages = forEachSystem ({ pkgs }: {
        juce-build-packages = with pkgs; [
          freetype
          fontconfig
          curl
          webkitgtk_4_1
          libsysprof-capture
          pcre2
          alsa-lib
          libxkbcommon
          xorg.libX11
          xorg.libXext
          xorg.libXcursor
          xorg.libXrandr
          xorg.libXinerama
          xorg.libXi
        ];

        juce = pkgs.stdenv.mkDerivation {
          name = "juce-8.0.8";
          src = juce-src;
          nativeBuildInputs = [ pkgs.cmake pkgs.pkg-config ];
          buildInputs = self.packages.${pkgs.system}.juce-build-packages
          ++ (with pkgs; pkgs.lib.optionals pkgs.stdenv.isLinux [
            # Linux-specific dependencies
            libjack2
            dbus
          ]);

          cmakeFlags = [
            "-DJUCE_BUILD_EXTRAS=OFF"
            "-DJUCE_BUILD_EXAMPLES=OFF"
          ];

          # Fix for pkg-config issues
          PKG_CONFIG_PATH = "${pkgs.alsa-lib}/lib/pkgconfig";

          configurePhase = ''
            runHook preConfigure
            cmake -B build -DCMAKE_INSTALL_PREFIX=$out -DCMAKE_BUILD_TYPE=Release
            runHook postConfigure
          '';

          buildPhase = ''
            runHook preBuild
            cmake --build build --parallel $NIX_BUILD_CORES
            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall
            cmake --install build
            ls $out
            runHook postInstall
          '';
        };
      });

      # Development shell
      devShells = forEachSystem ({ pkgs }: {
        default = pkgs.mkShell {
          name = "juce-development-shell";
          buildInputs = [
            pkgs.cmake
            pkgs.pkg-config
            self.packages.${pkgs.system}.juce
          ] ++ (with pkgs; [
            # Additional development tools + rust
            gcc
            gnumake
            ninja
            openssl
            rustc
            cargo
            libjack2
          ]) ++ self.packages.${pkgs.system}.juce-build-packages;

          # Environment variables for JUCE development
          shellHook = ''
            export JUCE_DRV_PATH=${self.packages.${pkgs.system}.juce}
            export JUCE_MODULES_PATH=${self.packages.${pkgs.system}.juce}/include/JUCE-8.0.8/modules
            echo "JUCE development shell ready!"
            echo "JUCE is available at: $JUCE_DRV_PATH"
            echo "JUCE modules at: $JUCE_MODULES_PATH"
          '';
        };
      });

      # Default package
      defaultPackage = forEachSystem ({ pkgs }: self.packages.${pkgs.system}.juce);
    };
}
