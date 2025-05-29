/* 회원가입 체크 */
$(document).ready(function () {
    let idChecked = false; // 아이디 중복 확인 여부
    let emailChecked = false; // 이메일 중복 확인 여부
    let codeChecked = false; // 회사 코드 확인 여부

    // 아이디 중복 확인 버튼 클릭 시
    $("#idCheckBtn").click(function () {
        let userId = $("#userId").val().trim();
        if (userId === "") {
            alert("아이디를 입력해 주세요.");
            return;
        }

        // AJAX 요청 (GET, Query Params 방식)
        $.ajax({
            type: "GET",
            url: "/primx/fun/sup/checkUserId.do",
            data: { userId: userId },
            contentType: "application/json; charset=utf-8",
            accept: "application/json",
            dataType: "json",
            success: function (response) {
                if (response.isAvailable) {
                    alert(response.message); // "사용 가능한 아이디입니다."
                    idChecked = true;
                } else {
                    alert(response.message); // "이미 사용 중인 아이디입니다."
                    idChecked = false;
                }
            },
            error: function (xhr, status, error) {
                console.error("### 아이디 중복 확인 중 오류 발생:", error);
                alert("아이디 중복 확인 중 오류가 발생했습니다.");
            }
        });
    });

    // 이메일 중복 확인 버튼 클릭 시
    $("#emailCheckBtn").click(function () {
        let email = $("#email").val().trim();

        // 이메일 형식 검증
        const emailPattern = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
        if (email === "") {
            alert("이메일을 입력해 주세요.");
            return;
        } else if (!emailPattern.test(email)) {
            alert("이메일 형식이 올바르지 않습니다.");
            $("#email").focus();
            return;
        }

        // 이메일 중복 확인 AJAX 요청 (GET, Query Params 방식)
        $.ajax({
            type: "GET",
            url: "/primx/fun/sup/checkEmail.do",
            data: { email: email },
            contentType: "application/json; charset=utf-8",
            accept: "application/json",
            dataType: "json",
            success: function (response) {
                if (response.isAvailable) {
                    alert(response.message); // "사용 가능한 이메일입니다."
                    emailChecked = true;
                } else {
                    alert(response.message); // "이미 사용 중인 이메일입니다."
                    emailChecked = false;
                }
            },
            error: function (xhr, status, error) {
                console.error("### 이메일 중복 확인 중 오류 발생:", error);
                alert("이메일 중복 확인 중 오류가 발생했습니다.");
            }
        });
    });

    // 회사 코드 확인 버튼 클릭 시
    $("#codeCheckBtn").click(function () {
        let companyCode = $("#companyCode").val().trim();
        if (companyCode === "") {
            alert("회사 코드를 입력해 주세요.");
            return;
        }
        // AJAX 요청 (GET, Query Params 방식)
        $.ajax({
            type: "GET",
            url: "/primx/fun/sup/checkCompany.do",
            data: { companyCode: companyCode },
            contentType: "application/json; charset=utf-8",
            accept: "application/json",
            dataType: "json",
            success: function (response) {
                if (response.isValid) {
                    alert(response.message); // "회사 코드가 확인되었습니다."
                    codeChecked = true;
                } else {
                    alert(response.message); // "유효하지 않은 회사 코드입니다."
                    codeChecked = false;
                }
            },
            error: function (xhr, status, error) {
                console.error("### 회사 코드 확인 중 오류 발생:", error);
                alert("회사 코드 확인 중 오류가 발생했습니다.");
            }
        });
    });

    // 입력값 변경 시 체크 상태 초기화
    $("#userId").on('input', function() {
        idChecked = false;
    });

    $("#email").on('input', function() {
        emailChecked = false;
    });

    $("#companyCode").on('input', function() {
        codeChecked = false;
    });

    // 회원가입 버튼 클릭 시
    $("#signUpBtn").click(function () {
        // 개별 필드 검사
        if ($("#userId").val().trim() === "") {
            alert("아이디를 입력해 주세요.");
            $("#userId").focus();
            return;
        }

        if ($("#userName").val().trim() === "") {
            alert("사용자 이름을 입력해 주세요.");
            $("#userName").focus();
            return;
        }

        let phoneNumber = $("#phoneNumber").val().trim();
        if (phoneNumber === "") {
            alert("전화번호를 입력해 주세요.");
            $("#phoneNumber").focus();
            return;
        } else if (!validatePhoneNumber(phoneNumber)) {
            alert("전화번호 형식이 올바르지 않습니다. 예: 010-1234-5678");
            $("#phoneNumber").focus();
            return;
        }

        let email = $("#email").val().trim();
        if (email === "") {
            alert("이메일을 입력해 주세요.");
            $("#email").focus();
            return;
        }

        // 이메일 형식 검증을 중복확인 버튼에서 처리함
        if (!emailChecked) {
            alert("이메일 중복 확인을 해주세요.");
            return;
        }

        if ($("#companyCode").val().trim() === "") {
            alert("회사 코드를 입력해 주세요.");
            $("#companyCode").focus();
            return;
        }

        let pw1 = $("#password").val().trim();
        if (pw1 === "") {
            alert("패스워드를 입력해 주세요.");
            $("#password").focus();
            return;
        }

        // 패스워드 유효성 검증
        if (!validatePassword(pw1)) {
            alert("패스워드는 최소 8자 이상이며, 영문/숫자/특수문자를 포함해야 합니다.");
            $("#password").focus();
            return;
        }

        let pw2 = $("#passwordConfirm").val().trim();
        if (pw2 === "") {
            alert("패스워드 확인을 입력해 주세요.");
            $("#passwordConfirm").focus();
            return;
        }

        if (pw1 !== pw2) {
            alert("패스워드와 패스워드 확인이 일치하지 않습니다.");
            $("#passwordConfirm").focus();
            return;
        }

        // 중복 확인 & 코드 입력 확인 여부 체크
        if (!idChecked) {
            alert("아이디 중복 확인을 해주세요.");
            return;
        }
        if (!codeChecked) {
            alert("회사 코드 확인을 해주세요.");
            return;
        }

        // 모든 검증이 통과되면 회원가입 요청
        let signUpData = {
            userId: $("#userId").val().trim(),
            userName: $("#userName").val().trim(),
            password: $("#password").val().trim(),
            email: $("#email").val().trim(),
            phoneNumber: $("#phoneNumber").val().trim(),
            companyCode: $("#companyCode").val().trim()
        };

        // 서버로 보내기 전 데이터 로그 확인
        console.log(signUpData);

        $.ajax({
            type: "POST",
            url: "/primx/fun/sup/signUp.do",
            data: JSON.stringify(signUpData),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            success: function(response) {
                if (response.success) {
                    alert("회원가입이 완료되었습니다. 로그인 페이지로 이동합니다.");
                    window.location.href = '/uat/uia/egovLoginUsr.do';
                } else {
                    console.log("서버 응답:", response); // 응답 로그
                    alert(response.message || "회원가입 중 오류가 발생했습니다.");
                }
            },
            error: function(xhr, status, error) {
                console.error("회원가입 처리 중 오류 발생:", error);
                alert("회원가입 처리 중 오류가 발생했습니다.");
            }
        });
    });
});
