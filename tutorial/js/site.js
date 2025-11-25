// site.js
// Uses jQuery for tab logic, highlight.js for syntax highlighting,
// and localStorage/cookies to remember theme and language across pages.

$(document).ready(function () {

  // ---------------- Storage helpers (localStorage + cookie fallback) ----------------

  function storageAvailable(type) {
    try {
      var storage = window[type];
      var testKey = "__storage_test__";
      storage.setItem(testKey, testKey);
      storage.removeItem(testKey);
      return true;
    } catch (e) {
      return false;
    }
  }

  var canUseLocalStorage = storageAvailable("localStorage");

  function setPref(key, value, days) {
    if (canUseLocalStorage) {
      window.localStorage.setItem(key, value);
    } else {
      // cookie fallback
      var expires = "";
      if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        expires = "; expires=" + date.toUTCString();
      }
      document.cookie = key + "=" + encodeURIComponent(value) + expires + "; path=/";
    }
  }

  function getPref(key) {
    if (canUseLocalStorage) {
      var v = window.localStorage.getItem(key);
      return v === null ? null : v;
    } else {
      // cookie fallback
      var nameEQ = key + "=";
      var ca = document.cookie.split(";");
      for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) === " ") c = c.substring(1, c.length);
        if (c.indexOf(nameEQ) === 0) {
          return decodeURIComponent(c.substring(nameEQ.length, c.length));
        }
      }
      return null;
    }
  }

  // ---------------- Theme handling ----------------

  var $themeSelect = $("#theme-select");
  var $hljsThemeLink = $("#hljs-theme");
  var themeBaseUrl = "https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/";
  var allowedThemes = ["atom-one-dark", "far", "atom-one-light"];

  function applyTheme(theme) {
    if (!$hljsThemeLink.length) return;
    if (allowedThemes.indexOf(theme) === -1) {
      theme = "atom-one-dark";
    }
    $hljsThemeLink.attr("href", themeBaseUrl + theme + ".min.css");

    if ($themeSelect.length) {
      $themeSelect.val(theme);
    }

    setPref("dsa_theme", theme, 365);
  }

  (function initTheme() {
    var themeCookieOrStorage = getPref("dsa_theme");
    var initialTheme = "atom-one-dark";

    if ($themeSelect.length) {
      if (themeCookieOrStorage && allowedThemes.indexOf(themeCookieOrStorage) !== -1) {
        initialTheme = themeCookieOrStorage;
      } else {
        initialTheme = $themeSelect.val() || "atom-one-dark";
      }
    } else if (themeCookieOrStorage && allowedThemes.indexOf(themeCookieOrStorage) !== -1) {
      initialTheme = themeCookieOrStorage;
    }

    applyTheme(initialTheme);

    if ($themeSelect.length) {
      $themeSelect.on("change", function () {
        var theme = $(this).val();
        applyTheme(theme);
      });
    }
  })();

  // ---------------- Global language tab handling ----------------

  var supportedLangs = ["c", "cpp", "java", "js", "cs", "py"];

  function applyLanguage(lang) {
    if (supportedLangs.indexOf(lang) === -1) {
      lang = "c";
    }

    // Update all tab buttons
    $(".tab-button").each(function () {
      var $btn = $(this);
      var isMatch = $btn.data("lang") === lang;
      $btn.toggleClass("active", isMatch);
    });

    // Update all panels
    $(".code-panel").each(function () {
      var $panel = $(this);
      var isMatch = $panel.data("lang-panel") === lang;
      $panel.toggleClass("hidden", !isMatch);
    });

    setPref("dsa_lang", lang, 365);
  }

  (function initLanguage() {
    if (!$(".code-example").length) {
      return;
    }

    var storedLang = getPref("dsa_lang");
    var initialLang;

    if (storedLang && supportedLangs.indexOf(storedLang) !== -1) {
      initialLang = storedLang;
    } else {
      var $activeBtn = $(".tab-button.active").first();
      if ($activeBtn.length) {
        initialLang = $activeBtn.data("lang");
      } else {
        var $firstBtn = $(".tab-button").first();
        initialLang = $firstBtn.length ? $firstBtn.data("lang") : "c";
      }
    }

    applyLanguage(initialLang);

    // Global click handler: selecting one tab updates all blocks
    $(document).on("click", ".tab-button", function (e) {
      e.preventDefault();
      var lang = $(this).data("lang");
      applyLanguage(lang);
    });
  })();

  // ---------------- Highlight.js init ----------------

  if (window.hljs && typeof hljs.highlightAll === "function") {
    hljs.highlightAll();
  }

  // ---------------- Deep dive modal behavior ----------------

  function openDeepDive(templateId) {
    var $template = $("#" + templateId);
    var $backdrop = $("#deep-dive-backdrop");
    if (!$template.length || !$backdrop.length) {
      return;
    }

    var title = $template.data("title") || "";
    var bodyHtml = $template.html();

    $backdrop.find("#deep-dive-title").text(title);
    $backdrop.find(".deep-dive-body").html(bodyHtml);

    $backdrop.addClass("is-open");
    $backdrop.attr("aria-hidden", "false");
  }

  function closeDeepDive() {
    var $backdrop = $("#deep-dive-backdrop");
    if (!$backdrop.length) {
      return;
    }
    $backdrop.removeClass("is-open");
    $backdrop.attr("aria-hidden", "true");
  }

  // Open on trigger click
  $(document).on("click", ".deep-dive-trigger", function (e) {
    e.preventDefault();
    var templateId = $(this).data("deepId");
    if (templateId) {
      openDeepDive(templateId);
    }
  });

  // Close button
  $(document).on("click", ".deep-dive-close", function (e) {
    e.preventDefault();
    closeDeepDive();
  });

  // Click on backdrop (but not modal content) closes
  $("#deep-dive-backdrop").on("click", function (e) {
    if (e.target === this) {
      closeDeepDive();
    }
  });

  // Escape key closes
  $(document).on("keydown", function (e) {
    if (e.key === "Escape" || e.key === "Esc") {
      closeDeepDive();
    }
  });

    // ---------------- Copy button for code blocks ----------------

  $(document).on("click", ".copy-button", function (e) {
    e.preventDefault();

    var $btn = $(this);
    var $example = $btn.closest(".code-example");

    if ($example.length === 0) return;

    // Determine current language from the global active tab
    var $activeTab = $(".tab-button.active").first();
    var lang = $activeTab.length ? $activeTab.data("lang") : "c";

    // Find the matching panel *within this example*
    var $panel = $example.find('.code-panel[data-lang-panel="' + lang + '"] code');
    if ($panel.length === 0) {
      // Fallback: just grab the first code block in this example
      $panel = $example.find("code").first();
      if ($panel.length === 0) return;
    }

    var codeText = $panel.text(); // .text() gives you decoded text (no HTML entities)

    function showCopiedState() {
      var originalText = $btn.text();
      $btn.addClass("copied").text("Copied!");
      setTimeout(function () {
        $btn.removeClass("copied").text(originalText);
      }, 1200);
    }

    // Prefer Clipboard API if available
    if (navigator.clipboard && navigator.clipboard.writeText) {
      navigator.clipboard.writeText(codeText)
        .then(showCopiedState)
        .catch(function () {
          // Fallback to execCommand
          var $temp = $("<textarea>")
            .css({ position: "fixed", left: "-9999px", top: "0" })
            .val(codeText)
            .appendTo("body");
          $temp[0].select();
          try {
            document.execCommand("copy");
          } catch (err) {
            console.error("Copy failed:", err);
          }
          $temp.remove();
          showCopiedState();
        });
    } else {
      // Legacy fallback immediately
      var $temp = $("<textarea>")
        .css({ position: "fixed", left: "-9999px", top: "0" })
        .val(codeText)
        .appendTo("body");
      $temp[0].select();
      try {
        document.execCommand("copy");
      } catch (err) {
        console.error("Copy failed:", err);
      }
      $temp.remove();
      showCopiedState();
    }
  });

});
