let translations = {}; 
let currentLang = localStorage.getItem("lang") || "ko";

// JSON 파일을 동적으로 불러오기 (언어 변경 함수)
async function loadLanguage(lang) {
    try {
        const responses = await Promise.all([
            fetch(`/Language/Common.${lang}.json`).then(res => res.json()),
           /* fetch(`/Language/Navigation.${lang}.json`).then(res => res.json()),
            fetch(`/Language/Gridcolumn.${lang}.json`).then(res => res.json()),
            fetch(`/Language/Dialog.${lang}.json`).then(res => res.json()),
            fetch(`/Language/Message.${lang}.json`).then(res => res.json()),
            fetch(`/Language/Popmenu.${lang}.json`).then(res => res.json()),*/
        ]);

        translations = Object.assign({}, ...responses);

        return translations;
    } catch (error) {
        console.error("X Translation file loading failed", error);
        return {};
    }
}

// 언어 변경 함수
function setLanguage(lang) {
    currentLang = lang;
    localStorage.setItem("lang", lang);

    let langToggle = document.getElementById("langToggle");
    if (langToggle) {
        langToggle.innerText = lang === "ko" ? "한국어" : "English";
    }

    loadLanguage(lang).then(() => {
        applyTranslations(); // JSON 로드 후 번역 적용
		//updateTimer(); // 타이머 UI도 번역 적용되도록 추가
    });
}

function getTranslation(keyPath) {
    let keys = keyPath.split(".");
    let value = translations;

    for (let key of keys) {
        if (value[key]) {
            value = value[key];
        } else {
            console.warn(`X Missing translation for key: ${keyPath}`);
            return keyPath; // 키가 없을 경우 그대로 반환
        }
    }
    return value;
}

// 번역 적용 함수
function applyTranslations() {

    document.querySelectorAll("[data-i18n]").forEach(element => {
        const keyPath = element.getAttribute("data-i18n");
        let value = translations;

        keyPath.split(".").forEach(key => {
            value = value[key] || keyPath; // 값이 없으면 keyPath 자체를 표시
        });

        element.innerText = value;
    });

    document.querySelectorAll("[data-i18n-placeholder]").forEach(element => {
        const keyPath = element.getAttribute("data-i18n-placeholder");
        let value = translations;

        keyPath.split(".").forEach(key => {
            value = value[key] || keyPath;
        });

        element.setAttribute("placeholder", value);
    });
    
    document.querySelectorAll("[data-i18n-tooltip]").forEach(element => {
        const keyPath = element.getAttribute("data-i18n-tooltip");
        let value = translations;

        keyPath.split(".").forEach(key => {
            value = value[key] || keyPath;
        });

        element.setAttribute("data-tooltip", value);
    });
}

// 페이지 로드 시 저장된 언어 적용 (새로고침 후 유지됨)
document.addEventListener("DOMContentLoaded", () => {
    setLanguage(currentLang);
});
