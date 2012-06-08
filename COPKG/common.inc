#define { author-version: "2.32.3"; }

#define { outercurve-publickeytoken: "1e373a58e25250cb"; }

#define { deps-publickeytoken: "${outercurve-publickeytoken}"; }

#define { libintl-dep-version: "0.18.1"; }

#define { libintl-dep-publickeytoken: "${deps-publickeytoken}"; }

#define { libffi-dep-version: "3.0.10"; }

#define { libffi-dep-publickeytoken: "${deps-publickeytoken}"; }

#define { zlib-dep-version: "1.2.5"; }

#define { zlib-dep-publickeytoken: "${outercurve-publickeytoken}"; }

license[lgpl] {
    license: "..\COPYING";
    license-url: "http://www.gnu.org/licenses/old-licenses/lgpl-2.0.html";
    license-type: "LGPL 2.0";
}

#define { general-description: "GLib is a general-purpose utility library for C, mostly used with GTK and GNOME."; }

metadata {
    author-version : "${author-version}";
    bug-tracker: "https://github.com/coapp-packages/glib/issues";
    stability : "0";
}
