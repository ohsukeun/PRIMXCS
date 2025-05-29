<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.requestAndApprovalSettings">PRIMX | 신청 및 결재 설정</title>
<form class="createRequestSetting" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.requestAndApprovalSettings"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.setting"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.requestAndApprovalSettings"></span>
			</h2>
		</div>
	</div>
    <div class="contDiv">
        <h3 class="contTitle" data-i18n="requestAndApprovalSettings.requestList">신청 목록</h3>
        <div class="tableWrap">
       		<!-- <div class="bu_wrap tableBtnPosAbsolute">
				<button class="addZoneBtn" type="button">+</button>
				<button class="" type="button">-</button>
			</div> -->
            <table class="dataTable checkDataTable" id="requestSettingTable">
                <thead>
                    <tr>
                        <th width="5%" data-i18n="requestAndApprovalSettings.grid.use">사용</th>
                        <th width="10%" data-i18n="requestAndApprovalSettings.grid.category">구분</th>
                        <th width="30%" data-i18n="requestAndApprovalSettings.grid.requestName">신청명</th>
                        <th width="40%" data-i18n="requestAndApprovalSettings.grid.approvalTarget">결재 대상</th>
                        <th width="10%" data-i18n="requestAndApprovalSettings.grid.modificationDate">수정일</th>
                    </tr>
                </thead>
                <tbody>
                    <!-- <tr>
                        <td></td>
                        <td>사용자</td>
                        <td>파일 반출 신청</td>
                        <td>연동 결재</td>
                        <td>2025. 1. 13. 오후 4:21:32</td>
                    </tr>
                    <tr>
                        <td></td>
                        <td>관리자</td>
                        <td>파일 반출 신청</td>
                        <td>사용자 결재</td>
                        <td>2025. 1. 13. 오후 4:21:32</td>
                    </tr>
                    <tr>
                        <td></td>
                        <td>사용자</td>
                        <td>계정 연장 신청</td>
                        <td>관리자 결재</td>
                        <td>2025. 1. 13. 오후 4:21:32</td>
                    </tr>
                    <tr>
                        <td></td>
                        <td>관리자</td>
                        <td>계정 연장 신청</td>
                        <td>관리자 결재</td>
                        <td>2025. 1. 13. 오후 4:21:32</td>
                    </tr>
                    <tr>
                        <td></td>
                        <td>사용자</td>
                        <td>사용자 등록 신청</td>
                        <td>관리자 결재</td>
                        <td>2025. 1. 13. 오후 4:21:32</td>
                    </tr>
                    <tr>
                        <td></td>
                        <td>관리자</td>
                        <td>사용자 등록 신청</td>
                        <td>관리자 결재</td>
                        <td>2025. 1. 13. 오후 4:21:32</td>
                    </tr> -->
                </tbody>
            </table>
        </div>
    </div>
    <div class="settingSection mb20 hide">
        <div class="flexWrap sbStart alignCenter mb20">
            <input type="checkbox" class="txtBox" id="useCheckbox">
            <span data-i18n="requestAndApprovalSettings.use">사용</span>
        </div>
        <div class="setWrap mb00">
            <div class="line">
                <div class="item">
                    <em data-i18n="requestAndApprovalSettings.approvalTarget">결재 대상</em>
                    <div>
                        <div class="selectBox">
                            <select class="drop" name="" id="approvTarget">
                                <option value="M" data-i18n="requestAndApprovalSettings.integrationApproval">연동 결재</option>
                                <option value="U" data-i18n="requestAndApprovalSettings.userApproval">사용자 결재</option>
                                <option value="A" data-i18n="requestAndApprovalSettings.adminApproval">관리자 결재</option>
                                <option value="AP" class="hide">결재자 결재</option>
                            </select>
                        </div>
                    </div>
                </div>
            </div>
            <div class="line" id="delegatedYnWrap">
                <div class="item">
                    <em data-i18n="requestAndApprovalSettings.batchApproval">전결 승인</em>
                    <div class="flexWrap alignCenter sbStart">
                        <input type="checkbox" class="txtBox" id="delegatedYn"><span data-i18n="requestAndApprovalSettings.use">사용</span>
                    </div>
                </div>
            </div>
            <div class="line">
                <div class="item">
                    <em data-i18n="requestAndApprovalSettings.announcement">공지 사항</em>
                    <div>
                        <input class="hide" type="text" id="noticeSeq" hidden>
                        <textarea name="" style="height: 200px;" data-i18n-placeholder="requestAndApprovalSettings.enterAnnouncement" id="noticeContent"></textarea>
                    </div>
                </div>
            </div>
            <div class="line">
                <div class="item">
                    <em><input type="checkbox" class="txtBox" id="reasonUsedYn"><span data-i18n="requestAndApprovalSettings.reasonRequired">사유 필수 입력</span></em>
                    <div>
                        <textarea name="" id="noticeReason" class="disable" style="height: 200px;" data-i18n-placeholder="requestAndApprovalSettings.enterDefaultReason" disabled></textarea>
                    </div>
                </div>
            </div>
        </div>
    </div>

	<div class="bu_wrap">
		<button class="saveBtn hide" type="button" onclick="saveSetting()" data-i18n="button.save">저장</button>
	</div>
</form>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/createRequestSetting.js' />"></script>