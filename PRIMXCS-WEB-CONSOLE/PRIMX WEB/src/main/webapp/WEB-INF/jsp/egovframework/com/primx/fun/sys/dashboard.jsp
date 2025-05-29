<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.dashboard">PRIMX | 대시보드</title>
<form class="dashboard" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.dashboard">대시보드</h2>
            <button type="button" class="refreshBtn" title="대시보드 새로고침" onClick="refreshDashboard()"></button>
		</div>
	</div>
    <div class="dashDivision">
        <div class="dashItem" onclick="loadMainContent('/primx/fun/pol/product/view.do');" style="cursor: pointer;">
            <div class="dashTitle" data-i18n="dashboard.productPolicy">제품 정책</div>
            <div class="dashboradContainer policy-bar-container">
                <div id="policy-bar">
                    <!-- JS로 각 영역이 동적으로 생성됩니다 -->
                </div>
                <div class="policy-tooltip">
                    <div class="tableWrap dashPolicyTableWrap">
                        <table class="dataTable" id="policyDataTable">
                            <thead>
                                <tr>
                                    <th width="30%" data-i18n="dashboard.grid.policyName">정책명</th>
                                    <th width="40%" data-i18n="dashboard.grid.policyID">정책 ID</th>
                                    <th width="15%" data-i18n="dashboard.grid.total">전체</th>
                                    <th width="15%" data-i18n="dashboard.grid.applied">적용</th>
                                </tr>
                            </thead>
                            <tbody>
                               
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
        <div class="dashItem">
            <div class="dashTitle" data-i18n="dashboard.licenseInstall">설치</div>
            <div class="dashboradContainer licenseItem flexWrap" style="cursor: pointer;" onclick="$('.topSetPopup').removeClass('hide');openSettingPopup();">
                <div class="legend-container">
                    <div class="legend-item not-used">
                        <div class="legend-color"></div>
                        <span class="legend-label" data-i18n="dashboard.unusedLicense">미사용 라이선스</span>
                        <span class="legend-value" id="notUsedCnt">0</span>
                        <span class="legend-total" id="totalCnt1">/ 0</span>
                    </div>
                    <div class="legend-item used">
                        <div class="legend-color"></div>
                        <span class="legend-label" data-i18n="dashboard.usedLicense">사용된 라이선스</span>
                        <span class="legend-value" id="usedCnt">0</span>
                        <span class="legend-total" id="totalCnt2">/ 0</span>
                    </div>
                </div>
                <div class="chart-container">
                    <div class="chart-bg"></div>
                    <div class="chart-center-text" id="centerText">0.0%</div>
                    <canvas id="licenseChart"></canvas>
                </div>
            </div>
        </div>
        <div class="dashItem" onclick="loadMainContent('/primx/fun/sys/systemStatus/view.do');" style="cursor: pointer;">
            <div class="dashTitle" data-i18n="dashboard.systemStatus">시스템 현황</div>
            <div class="dashboradContainer systemStatusItem">
                <ul class="top-list">
                    <li class="cpuLi">
                        <p class="tit">CPU</p>
                        <div class="cont">
                            <span class="value" id="cpuStat">0</span>
                            <span class="unit">%</span>
                        </div>
                    </li>
                    <li>
                        <p class="tit">MEMORY</p>
                        <div class="cont">
                            <span class="value" id="memStat">0</span>
                            <span class="unit">/</span>
                            <span class="unit" id="memTotal">0</span>
                            <span class="unit">GB</span>
                        </div>
                    </li>
                    <li>
                        <p class="tit">DISK</p>
                        <div class="cont">
                            <span class="value" id="diskStat">0</span>
                            <span class="unit">/</span>
                            <span class="unit" id="diskTotal">0</span>
                            <span class="unit">GB</span>
                        </div>
                    </li>
                </ul>
                <div class="bot-list">
                    <ul class="left-list">
                        <li>
                            <p class="tit" data-i18n="dashboard.user">사용자</p>
                            <div class="cont">
                                <span class="value" id="userPcCount">0</span>
                            </div>
                        </li>
                        <li>
                            <p class="tit" data-i18n="dashboard.public">공용</p>
                            <div class="cont">
                                <span class="value" id="commonPcCount">0</span>
                            </div>
                        </li>
                    </ul>
                    <ul class="right-list">
                        <li>
                            <p class="tit" data-i18n="dashboard.unconnectedSystem">미접속 시스템</p>
                            <div class="cont">
                                <span class="value" id="notConnectedPcCount">0</span>
                            </div>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
        <div class="dashItem">
            <div class="dashTitle" data-i18n="dashboard.requestAndApproval">신청 및 결재</div>
            <div class="dashboradContainer requestAndApprovalItem">
                <ul class="top-list" onclick="loadMainContent('/primx/fun/rqt/createRequest/view.do');" style="cursor: pointer;">
                    <li>
                        <p class="tit" data-i18n="dashboard.request">신청</p>
                        <div class="cont">
                            <span class="value" id="requestCount">0</span>
                        </div>
                    </li>
                    <li>
                        <p class="tit" data-i18n="dashboard.fileMove">이동 파일</p>
                        <div class="cont">
                            <span class="value" id="moveFileCount">0</span>
                        </div>
                    </li>
                    <li>
                        <p class="tit" data-i18n="dashboard.accountExtension">계정 연장</p>
                        <div class="cont">
                            <span class="value" id="accountExtendCount">0</span>
                        </div>
                    </li>
                    <li>
                        <p class="tit" data-i18n="dashboard.userRegistration">사용자 등록</p>
                        <div class="cont">
                            <span class="value" id="userRegistCount">0</span>
                        </div>
                    </li>
                </ul>
                <ul class="bot-list" onclick="loadMainContent('/primx/fun/rqt/requestApproval/view.do');" style="cursor: pointer;">
                    <li>
                        <p class="tit" data-i18n="dashboard.approval">결재</p>
                        <div class="cont">
                            <span class="value" id="approvalCount">0</span>
                        </div>
                    </li>
                    <li>
                        <p class="tit" data-i18n="dashboard.approve">승인</p>
                        <div class="cont">
                            <span class="value" id="approvalAllowCount">0</span>
                        </div>
                    </li>
                    <li>
                        <p class="tit" data-i18n="dashboard.reject">반려</p>
                        <div class="cont">
                            <span class="value" id="rejectCount">0</span>
                        </div>
                    </li>
                </ul>
            </div>
        </div>
        <div class="dashItem">
            <div class="dashTitle" data-i18n="dashboard.fileMoveHistory">이동 파일</div>
            <div class="dashboradContainer fileMoveHistoryItem flexWrap">
                <ul class="legend-container">
                    <li>
                        <span class="new"></span>
                        <p data-i18n="dashboard.create">생성</p>
                    </li>
                    <li>
                        <span class="used"></span>
                        <p data-i18n="dashboard.use">사용</p>
                    </li>
                    <li>
                        <span class="expired"></span>
                        <p data-i18n="dashboard.expire">만료</p>
                    </li>
                </ul>
                <div class="chart-container">
                    <div class="chartWrap" onclick="loadMainContent('/primx/fun/file/view.do');" style="cursor: pointer;">
                        <canvas id="fileMoveChart"></canvas>
                    </div>
                    <div class="chart-period-selector" id="chart-period-selector">
                        <label><input type="radio" name="period" value="day" checked data-i18n="dashboard.day"><span data-i18n="dashboard.day">일</span></label>
                        <label><input type="radio" name="period" value="week" data-i18n="dashboard.week"><span data-i18n="dashboard.week">주</span></label>
                        <label><input type="radio" name="period" value="month" data-i18n="dashboard.month"><span data-i18n="dashboard.month">월</span></label>
                    </div>
                </div>
            </div>
        </div>
        <!--
        E00001	KOR	실행 오류
        P01001	KOR	명령 실행
        P01002	KOR	엑세스 리스트 색인 생성
        P01003	KOR	엑세스 리스트 폴더 생성
        P02001	KOR	파일 암복호화
        P02002	KOR	폴더 암복호화
        P02003	KOR	암호 영역 검사
        P02004	KOR	암호 영역 접근
        P02005	KOR	영역 암호화
        P02006	KOR	암호 파일 접근
        P02007	KOR	공유 폴더 갱신
        P02008	KOR	시스템 동작
        P02009	KOR	파일 변경
        P02010	KOR	영역 업데이트
        P02011	KOR	접근 목록 파일 업데이트
        P02012	KOR	접근 목록 파일 접근
        P02013	KOR	세션 업데이트
        P02014	KOR	내보내기
        P02015	KOR	인증서 파일 접근
        P02016	KOR	SSO 동작
        P02017	KOR	접근 키 업데이트
        P02018	KOR	접근키 접근
        P02019	KOR	복구 동작
        P02020	KOR	그룹 정책 업데이트
        P02021	KOR	컨테이너
        W00001	KOR	실행 경고
        
        -->
        <div class="dashItem">
            <div class="dashTitle flexWrap alignCenter">
                <span data-i18n="dashboard.productHistory">제품 이력</span>
                <div class="dropMenu_dashboard">
                    <p class="hamberger selectMenu" onclick="dropMenuHamberger();">
                        <span></span>
                        <span></span>
                        <span></span>
                    </p>
                    <ul>
                        <li>
                            <input type="checkbox" name="" id="" value="E00001">
                            <label for="" data-i18n="search.option.executionError">실행 오류</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P01001">
                            <label for="" data-i18n="search.option.commandExecution">명령 실행</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P01002">
                            <label for="" data-i18n="search.option.accessListIndexCreation">엑세스 리스트 색인 생성</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P01003">
                            <label for="" data-i18n="search.option.accessListFolderCreation">엑세스 리스트 폴더 생성</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02001">
                            <label for="" data-i18n="search.option.fileEncryptionDecryption">파일 암복호화</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02002">
                            <label for="" data-i18n="search.option.folderEncryptionDecryption">폴더 암복호화</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02003">
                            <label for="" data-i18n="search.option.cryptoAreaCheck">암호 영역 검사</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02004">
                            <label for="" data-i18n="search.option.cryptoAreaAccess">암호 영역 접근</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02005">
                            <label for="" data-i18n="search.option.areaEncryption">영역 암호화</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02006">
                            <label for="" data-i18n="search.option.cryptoFileAccess">암호 파일 접근</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02007">
                            <label for="" data-i18n="search.option.sharedFolderUpdate">공유 폴더 갱신</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02008">
                            <label for="" data-i18n="search.option.systemOperation">시스템 동작</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02010">
                            <label for="" data-i18n="search.option.areaUpdate">영역 업데이트</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02011">
                            <label for="" data-i18n="search.option.accessListFileUpdate">접근 목록 파일 업데이트</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02012">
                            <label for="" data-i18n="search.option.accessListFileAccess">접근 목록 파일 접근</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02013">
                            <label for="" data-i18n="search.option.sessionUpdate">세션 업데이트</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02014">
                            <label for="" data-i18n="search.option.export">내보내기</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02015">
                            <label for="" data-i18n="search.option.certificateFileAccess">인증서 파일 접근</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02016">
                            <label for="" data-i18n="search.option.SSOOperation">SSO 동작</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02017">
                            <label for="" data-i18n="search.option.accessKeyUpdate">접근 키 업데이트</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02018">
                            <label for="" data-i18n="search.option.accessKeyAccess">접근 키 접근</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02019">
                            <label for="" data-i18n="search.option.recoveryOperation">복구 동작</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02020">
                            <label for="" data-i18n="search.option.groupPolicyUpdate">그룹 정책 업데이트</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="P02021">
                            <label for="" data-i18n="search.option.container">컨테이너</label>
                        </li>
                        <li>
                            <input type="checkbox" name="" id="" value="W00001">
                            <label for="" data-i18n="search.option.executionWarning">실행 경고</label>
                        </li>
                    </ul>
                </div>
            </div>
            <div class="tableWrap dashTableWrap" onclick="loadMainContent('/primx/fun/log/productLog/view.do');" style="cursor: pointer;">
                <table class="dataTable checkDataTable" id="productLogDataTable">
                    <thead>
                        <tr>
                            <th width="5%" data-i18n="productHistory.grid.date">일자</th>
                            <th width="5%" data-i18n="productHistory.grid.time">시각</th>
                            <th width="5%" data-i18n="productHistory.grid.level">레벨</th>
                            <th width="20%" data-i18n="productHistory.grid.kind">종류</th>
                            <th width="10%" data-i18n="productHistory.grid.userName">사용자명</th>
                        </tr>
                    </thead>
                    <tbody>
                        <!-- 데이터는 DataTables에 의해 동적으로 채워집니다 -->
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</form>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/dashboard.js' />"></script>