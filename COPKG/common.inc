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

/* To use self-built dependencies, use ${Package.PublicKeyToken} instead of ${outercurve-publickeytoken} */
#define { outercurve-publickeytoken: "1e373a58e25250cb"; }

#define { libintl-version: 0.18.1.3; }

#define { libintl-publickeytoken: "${outercurve-publickeytoken}"; }
