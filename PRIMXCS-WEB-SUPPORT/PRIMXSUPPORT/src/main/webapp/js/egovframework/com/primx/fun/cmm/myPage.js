/* 마이페이지 정보 조회 */
$(document).ready(function() {
    // 사용자 정보 조회
    function getUserInfo() {
        $.ajax({
            url: '/primx/fun/cmm/getMyInfo.do',
            type: 'GET',
            headers: {
                'Authorization': 'Bearer ' + localStorage.getItem('token')
            },
            success: function(response) {
                if (response) {
                    $('#userId').val(response.userId);
                    $('#email').val(response.email);
                    $('#phoneNumber').val(response.phoneNumber);
                    $('#userName').val(response.userName);
                    $('#companyCode').val(response.companyCode);
                }
            },
            error: function(xhr, status, error) {
                console.error('사용자 정보 조회 실패:', error);
                alert('사용자 정보를 불러오는데 실패했습니다.');
            }
        });
    }

    // 페이지 로드 시 사용자 정보 조회
    getUserInfo();

    // 수정 버튼 클릭 이벤트
    $('.editBtn').click(function() {
        $('input.yEdit').prop('disabled', false).removeClass("disabled");
        $('.pwSection').removeClass('hide');
        $(this).addClass('hide');
        $('.saveBtn').removeClass('hide');
    });

    // 저장 버튼 클릭 이벤트
    $('.saveBtn').click(function() {
        // 전화번호 검증
        const phoneNumber = $('#phoneNumber').val().trim();
        if (phoneNumber === '') {
            alert('전화번호를 입력해주세요.');
            $('#phoneNumber').focus();
            return;
        }
        if (!validatePhoneNumber(phoneNumber)) {
            alert('전화번호 형식이 올바르지 않습니다. 예: 010-1234-5678');
            $('#phoneNumber').focus();
            return;
        }

        // 사용자명 검증
        const userName = $('#userName').val().trim();
        if (!userName) {
            alert('사용자명을 입력해주세요.');
            $('#userName').focus();
            return;
        }

        // 서버로 전송할 데이터 구성
        const updateData = {
            userId: $('#userId').val().trim(),
            phoneNumber: phoneNumber,
            userName: userName
        };

        // 패스워드 변경 검증
        const currentPassword = $('#currentPassword').val().trim();
        const newPassword = $('#newPassword').val().trim();
        const confirmPassword = $('#confirmPassword').val().trim();

        // 패스워드 필드 중 하나라도 입력된 경우
        if (currentPassword || newPassword || confirmPassword) {
            // 현재 비밀번호 검증
            if (!currentPassword) {
                alert('현재 비밀번호를 입력해주세요.');
                $('#currentPassword').focus();
                return;
            }

            // 새 비밀번호 검증
            if (!newPassword) {
                alert('새 비밀번호를 입력해주세요.');
                $('#newPassword').focus();
                return;
            }

            // 새 비밀번호 확인 검증
            if (!confirmPassword) {
                alert('새 비밀번호 확인을 입력해주세요.');
                $('#confirmPassword').focus();
                return;
            }

            // 현재 비밀번호와 새 비밀번호가 같은 경우
            if (currentPassword === newPassword) {
                alert('새 비밀번호는 현재 비밀번호와 다르게 입력해주세요.');
                $('#newPassword').focus();
                return;
            }

            // 신규 패스워드 유효성 검증
            if (!validatePassword(newPassword)) {
                alert('새 비밀번호는 최소 8자 이상이며, 영문/숫자/특수문자를 포함해야 합니다.');
                $('#newPassword').focus();
                return;
            }

            // 신규 패스워드 일치 검증
            if (newPassword !== confirmPassword) {
                alert('새 비밀번호와 확인 비밀번호가 일치하지 않습니다.');
                $('#confirmPassword').focus();
                return;
            }

            // 패스워드 관련 데이터 추가
            updateData.currentPassword = currentPassword;
            updateData.password = newPassword;
            updateData.confirmPassword = confirmPassword;
        }

        // 서버로 데이터 전송
        $.ajax({
            url: '/primx/fun/cmm/myPageEdit.do',
            type: 'PUT',
            contentType: 'application/json',
            data: JSON.stringify(updateData),
            success: function(response) {
                if (response.success) {
                    alert(response.message);
                    $('input.yEdit').prop('disabled', true).addClass("disabled");
                    $('.pwSection').addClass('hide');
                    $('.saveBtn').addClass('hide');
                    $('.editBtn').removeClass('hide');
                    // 패스워드 필드 초기화
                    $('#currentPassword, #newPassword, #confirmPassword').val('');
                    // 변경된 정보 다시 조회
                    getUserInfo();
                } else {
                    alert(response.message);
                    // 비밀번호 관련 오류인 경우 해당 필드에 포커스
                    if (response.message.includes('현재 비밀번호')) {
                        $('#currentPassword').focus();
                    } else if (response.message.includes('새 비밀번호')) {
                        $('#newPassword').focus();
                    }
                }
            },
            error: function(xhr, status, error) {
                console.error('회원 정보 수정 실패:', error);
                
                try {
                    const errorResponse = JSON.parse(xhr.responseText);
                    if (errorResponse.message) {
                        alert(errorResponse.message);
                        // 비밀번호 관련 오류인 경우 해당 필드에 포커스
                        if (errorResponse.message.includes('현재 비밀번호')) {
                            $('#currentPassword').focus();
                        } else if (errorResponse.message.includes('새 비밀번호')) {
                            $('#newPassword').focus();
                        }
                    } else {
                        alert('회원 정보 수정에 실패했습니다.');
                    }
                } catch (e) {
                    alert('회원 정보 수정에 실패했습니다.');
                }
            }
        });
    });
});