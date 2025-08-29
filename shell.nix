{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "juce-audio-plugin-dev";

  buildInputs = with pkgs; [
    # JUCE dependencies
    freetype
    fontconfig
    curl
    webkitgtk_4_1  # For webkit-based components :cite[1]
    libsysprof-capture # still figuring it out
    pcre2

    # X11 libraries for GUI components
    xorg.libX11
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXext
    xorg.libXcursor

    # Audio and MIDI dependencies
    alsa-lib       # ALSA support for Linux audio
    jack2          # JACK audio support

    # Plugin format dependencies (optional)
    # vst2sdk        # VST2 SDK (if needed; may require manual handling)
    # vst3sdk        # VST3 SDK (if needed; may require manual handling)

    # Development tools
    cmake
    pkg-config     # Necessary for locating headers/libs :cite[1]:cite[10]
    gcc
#    binutils
    gnumake
  ];


  # Environment variables to help compilers find dependencies
  env = {
    # Point pkg-config to necessary libraries
    PKG_CONFIG_PATH = "${pkgs.webkitgtk_4_1}/lib/pkgconfig:${pkgs.alsa-lib}/lib/pkgconfig";
  };

  # Shell hook for setup (optional)
  shellHook = ''
    echo "Entering JUCE audio plugin development environment"
    # Add any custom setup commands here, e.g., setting SDK paths
    export PKG_CONFIG_PATH="${pkgs.webkitgtk_4_1}/lib/pkgconfig:${pkgs.alsa-lib}/lib/pkgconfig";
  '';
}
