<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.productDefaultPolicySettings">PRIMX | 제품 기본 정책 설정</title>
<form class="productPolicySettings" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.productDefaultPolicySettings"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.setting"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.productDefaultPolicySettings"></span>
			</h2>
		</div>
		<div class="selectBox mb15">
			<select class="drop" name="productSelect" id="productSelect">
				<!-- 제품 목록이 여기에 추가됩니다. -->
			</select>
		</div>
	</div>
    <div class="zonecentralSection">
        <div class="contDiv hide">
            <h3 class="contTitle" data-i18n="productDefaultPolicySettings.policyScript">정책 스크립트</h3>
            <div class="tableWrap">
                <table id="policyScriptTable">
                    <thead>
                        <tr>
                            <th width="3%" data-i18n="productDefaultPolicySettings.grid.number">번호</th>
                            <th width="50%" data-i18n="productDefaultPolicySettings.grid.scriptName">스크립트명</th>
                            <th width="40%" data-i18n="productDefaultPolicySettings.grid.file">파일명</th>
                            <th width="7%" data-i18n="productDefaultPolicySettings.grid.up">업로드</th>
                        </tr>
                    </thead>
                    <tbody>
                        <!-- 스크립트 데이터가 여기에 추가됩니다. -->
                    </tbody>
                </table>
            </div>
        </div>
        <div class="contDiv">
            <h3 class="contTitle" data-i18n="productDefaultPolicySettings.path">경로</h3>
            <ul class="setWrap">
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.personalZafPath120">개인Zaf 경로 (120)</em>
                        <div>
                            <input type="text" class="txtBox" id="default120" value="">
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.zafPath121">Zaf 경로 (121)</em>
                        <div>
                            <input type="text" class="txtBox" id="default121" value="">
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.zafCachePath122">Zaf Cache 경로 (122)</em>
                        <div>
                            <input type="text" class="txtBox" id="default122" value="">
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.backupPath127">백업 경로 (127)</em>
                        <div>
                            <input type="text" class="txtBox" id="default127" value="">
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.logFilePath309">로그파일 경로 (309)</em>
                        <div>
                            <input type="text" class="txtBox" id="default309" value="">
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.zonemapPath343">Zonemap 경로 (343)</em>
                        <div>
                            <input type="text" class="txtBox" id="default343" value="">
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.keymapPath345">Keymap 경로 (345)</em>
                        <div>
                            <input type="text" class="txtBox" id="default345" value="">
                        </div>
                    </div>
                </li>
            </ul>
        </div>
        <div class="contDiv">
            <h3 class="contTitle" data-i18n="productDefaultPolicySettings.fixedAndReserved">고정 및 예약</h3>
            <h4 class="contSubTitle" data-i18n="productDefaultPolicySettings.requiredZaf131">필수 Zaf (131)</h4>
            <div class="tableWrap">
                <table id="default131">
                    <thead>
                        <tr>
                        	<th width="10%" data-i18n="productDefaultPolicySettings.grid.category">분류</th>
                            <th width="10%" data-i18n="productDefaultPolicySettings.grid.name">이름</th>
                            <th width="50%" data-i18n="productDefaultPolicySettings.grid.value">값</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                        	<td>Mandatory</td>
                            <td><input type="text" name=""  class="txtBox w100Per" value=""></td>
                            <td>
	                            <div class="inputFileWrap">
									<input class="inputFile" id="uploadundefined" type="file" multiple="" style="display: none;">
									<span class="spanFile" id="spanFileundefined"></span>
									<label class="labelFile" for="uploadundefined" data-i18n="button.upload">업로드</label>
									<span class="fileName" id="fileNameundefined"></span>
								</div>
							</td>
                        </tr>
                        <tr>
                        	<td>Helpdesk</td>
                            <td><input type="text" name=""  class="txtBox w100Per" value=""></td>
                            <td>
	                            <div class="inputFileWrap">
									<input class="inputFile" id="uploadundefined" type="file" multiple="" style="display: none;">
									<span class="spanFile" id="spanFileundefined"></span>
									<label class="labelFile" for="uploadundefined" data-i18n="button.upload">업로드</label>
									<span class="fileName" id="fileNameundefined"></span>
								</div>
							</td>
                        </tr>
                    </tbody>
                </table>
            </div>
            <h4 class="contSubTitle" data-i18n="productDefaultPolicySettings.reserved499">예약 (499)</h4>
            <div class="tableWrap">
                <table id="default499">
                    <thead>
                        <tr>
                            <th width="30%" data-i18n="productDefaultPolicySettings.grid.name">이름</th>
                            <th width="70%" data-i18n="productDefaultPolicySettings.grid.value">값</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td><input type="text" name=""  class="txtBox w100Per" value="ApplyKillProc"></td>
                            <td><input type="text" name=""  class="txtBox w100Per_10" value="msedgewebview2.exe={startorder=1;CloseInQuietMode=1;Neverask=1};"></td>
                        </tr>
                    </tbody>
                </table>
            </div>
            <h4 class="contSubTitle" data-i18n="productDefaultPolicySettings.disablePersonalPolicy2">개인정책 비활성화 (2)</h4>
            <div class="selectBox">
                <select class="drop" name="" id="default2" >
                    <option data-i18n="boolean.y" value="Y">예</option>
                    <option data-i18n="boolean.n" value="N">아니오</option>
                </select>
            </div>
        </div>
        <div class="contDiv">
            <h3 class="contTitle" data-i18n="productDefaultPolicySettings.agentIntegration">Agent 연동</h3>
            <ul class="setWrap">
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.productTrayHide197">제품 트레이 숨김 (197)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default197" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.disableAPI392">API 사용금지 (392)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default392" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
            </ul>
        </div>
        <div class="contDiv">
            <h3 class="contTitle" data-i18n="productDefaultPolicySettings.accessType">접근 유형</h3>
            <ul class="setWrap">
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.disablePKCS12KeyFileAccess103">암호화된 영역/컨테이너 PKCS#12 키 파일 접근 비활성화(103)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default103" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.disableCSPProviderAccess105">암호화된 영역/컨테이너 CSP 공급자 접근 비활성화(105)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default105" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
            </ul>
        </div>
        <div class="contDiv">
            <h3 class="contTitle" data-i18n="productDefaultPolicySettings.personalAccessList">개인접근 목록</h3>
            <ul class="setWrap">
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.personalAccessList123">개인접근 목록(123)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default123" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.disableCreatePersonalAccessList124">사용자 개인 접근 목록 생성 불가(124)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default124" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.syncPersonalAccessListWithDefaultLocation125">개인 접근 목록을 기본 위치와 동기화(125)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default125" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.personalAccessListNameFormat126">개인 접근 목록 이름 형식 (126)</em>
                        <div>
                            <input type="text" class="txtBox" id="default126" value="">
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.createPersonalAccessListOnSessionStart5120">세션 시작시 개인 접근 목록 생성(5120)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default5120" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
            </ul>
        </div>
        <div class="contDiv">
            <h3 class="contTitle" data-i18n="productDefaultPolicySettings.logs">로그</h3>
            <ul class="setWrap">
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.logOpenCloseAreaEvents302">영역 열기 및 닫기 이벤트에 대한 로깅 켜기(302)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default302" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.logAccessToEncryptedFiles306">암호화된 파일에 대한 접근 로그(306)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default306" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.logDeniedAccessToEncryptedFiles307">암호화된 파일에 대한 거부된 접근 기록(307)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default307" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
                <li class="line">
                    <div class="item">
                        <em data-i18n="productDefaultPolicySettings.logDeniedCreationOfPlainTextFiles308">일반 텍스트 파일 생성이 거부된 로그(308)</em>
                        <div>
                            <div class="selectBox">
                                <select class="drop" name="" id="default308" >
                                    <option data-i18n="boolean.y" value="Y">예</option>
                                    <option data-i18n="boolean.n" value="N">아니오</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </li>
            </ul>
        </div>
    </div>
	<div class="bu_wrap">
		<button type="button" id="saveBtn" data-i18n="button.save" onclick="saveModifiedPolicies()">저장</button>
	</div>
</form>

<!-- 업로드 팝업 -->
<section class="popup fileUploadPopup hide">
    <div class="bgscreen"></div>
    <div class="pop_wrap">
        <strong class="pop_head" data-i18n="productDefaultPolicySettings.fileUploadPopup.popTitle">정책 스크립트 업로드</strong>
        <form action="">
            <div class="pop_cont">
                <ul class="setWrap mb00">
                    <input type="hidden" class="hide" name="rowId" id="rowId" value="">
                    <input type="hidden" class="hide" name="seqNo" id="seqNo" value="">
                    <li class="line">
                        <div class="item">
                            <em data-i18n="productDefaultPolicySettings.fileUploadPopup.script">스크립트명</em>
                            <div class=""><input type="text" name="program" id="scriptName" class="txtBox w100Per" value="" readonly disabled></div>
                        </div>
                    </li>
                    <li class="line">
                        <div class="item">
                            <em data-i18n="productDefaultPolicySettings.fileUploadPopup.file">파일</em>
                            <div class="">
                                <div class="inputFileWrap">
                                    <input class="inputFile" id="upload1" type="file" multiple="">
                                    <span class="spanFile" data-i18n="button.uploadFile" id="fileName">파일 업로드</span>
                                    <label class="labelFile w150" for="upload1" data-i18n="button.fileSelection">파일선택</label>
                                </div>
                            </div>
                        </div>
                    </li>
                </ul>
                <div class="bu_wrap">
                    <button type="button" class="" data-i18n="button.confirm" onclick="uploadFile()">확인</button>
                    <button type="button" class="lineBtn" data-i18n="button.cancel"onclick="$('.fileUploadPopup').addClass('hide');">취소</button>
                </div>
            </div>
        </form>
        <button type="button" class="bu_close_pop"></button>
    </div>
</section>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/productPolicySetting.js' />"></script>