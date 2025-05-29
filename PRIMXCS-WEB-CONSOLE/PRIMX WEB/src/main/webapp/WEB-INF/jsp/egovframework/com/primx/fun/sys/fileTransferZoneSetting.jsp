<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.fileTransferZoneSettings">PRIMX | 파일전송 Zone 설정</title>
<form class="fileTransferZoneSetting" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.fileTransferZoneSettings"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.setting"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.fileTransferZoneSettings"></span>
			</h2>
		</div>
	</div>
	<div class="contDiv">
		<h3 class="contTitle" data-i18n="fileTransferZoneSettings.fileTransferZoneList">파일전송 Zone 목록</h3>
		<div class="tableWrap">
			<div class="bu_wrap tableBtnPosAbsolute">
				<button class="addZoneBtn" type="button" onclick="addFileTransferZonePopup()">+</button>
				<button class="" type="button" onclick="deleteFileZone()">-</button>
			</div>
	        <table class="dataTable checkDataTable" id="fileZoneTable">
	            <thead>
	                <tr>
	                    <th width="3%" data-i18n="fileTransferZoneSettings.grid.select">선택</th>
	                    <th width="20%" data-i18n="fileTransferZoneSettings.grid.protocol">프로토콜</th>
	                    <th width="77%" data-i18n="fileTransferZoneSettings.grid.name">이름</th>
	                </tr>
	            </thead>
	            <tbody>
	               <!--  <tr>
	                    <td></td>
	                    <td>HTTP curl</td>
	                    <td>PRIMX 파일 반출</td>
	                </tr>
	                <tr>
	                    <td></td>
	                    <td>FTP curl</td>
	                    <td>영업부 NAS</td>
	                </tr>
	                <tr>
	                    <td></td>
	                    <td>HTTPS</td>
	                    <td>Groupware 결산</td>
	                </tr> -->
	            </tbody>
	        </table>
	    </div>
	</div>
</form>

<!-- 파일전송 zone 추가 -->
<section class="popup addFileTransferZonePopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="fileTransferZoneSettings.addFileTransferZonePopup.popTitle">파일전송 Zone</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em data-i18n="fileTransferZoneSettings.addFileTransferZonePopup.zoneName">Zone명</em>
							<div>
								<input type="hidden" id="zoneId">
                                <input type="text" class="txtBox" id="zoneNm">
                            </div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="fileTransferZoneSettings.addFileTransferZonePopup.protocol">프로토콜</em>
							<div>
                                <div class="selectBox mr00 w100Per">
                                    <select class="drop" name="" id="proto">
                                        <option value="http">HTTP curl</option>
                                        <option value="https">HTTPS curl</option>
                                        <option value="ftp">FTP curl</option>
                                        <option value="sftp">SFTP curl</option>
                                    </select>
                                </div>
                            </div>
						</div>
					</li>
				</ul>
                <div class="fileZoneTabWrap">
					<ul class="tabTitle">
						<li class="on" data-i18n="fileTransferZoneSettings.addFileTransferZonePopup.script">스크립트</li>
						<li data-i18n="fileTransferZoneSettings.addFileTransferZonePopup.input">입력</li>
					</ul>
					<ul class="tabCont">
						<li class="on">
                            <div class="tableWrap">
                                <div class="bu_wrap mb10">
                                    <button class="addZoneBtn" type="button" onclick="addScriptPopup()">+</button>
                                    <button class="" type="button" onclick="deleteScriptPopup()">-</button>
                                </div>
                                <table class="dataTable checkDataTable" id="scriptTable">
                                    <thead>
                                        <tr>
                                        	<th></th>
                                            <th width="10%">Usage</th>
                                            <th width="90%">Script</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <%-- <tr>
                                        	<td></td>
                                            <td><input type="text" class="txtBox w100Per" value="-X POST"></td>
                                            <td><input type="text" class="txtBox w100Per_10" value='"http://192.168.0.49:81/api/auth/user/login"'></td>
                                        </tr>
                                        <tr>
                                        	<td></td>
                                            <td><input type="text" class="txtBox w100Per" value="-H"></td>
                                            <td><input type="text" class="txtBox w100Per_10" value='"Content-Type: application/json"'></td>
                                        </tr>
                                        <tr>
                                        	<td></td>
                                            <td><input type="text" class="txtBox w100Per" value="-H"></td>
                                            <td><input type="text" class="txtBox w100Per_10" value='"Accept: application/json"'></td>
                                        </tr>
                                        <tr>
                                        	<td></td>
                                            <td><input type="text" class="txtBox w100Per" value="-d"></td>
                                            <td><input type="text" class="txtBox w100Per_10" value='-d "{\"username\": \"<UserID>\", \"password\": \"<Pass>\"}"'></td>
                                        </tr> --%>
                                    </tbody>
                                </table>
                            </div> 
						</li>
                        <li>
                            <div class="tableWrap">
                                <div class="bu_wrap mb10">
                                    <button class="addZoneBtn" type="button" onclick="addInputPopup()">+</button>
                                    <button class="" type="button" onclick="deleteInputPopup()">-</button>
                                </div>
                                <table class="dataTable checkDataTable" id="inputTable">
                                    <thead>
                                        <tr>
                                            <th width="10%"></th>
                                            <th width="30%">Param</th>
                                            <th width="60%" data-i18n="fileTransferZoneSettings.addFileTransferZonePopup.name">이름</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <%-- <tr>
                                            <td>1</td>
                                            <td><input type="text" class="txtBox w100Per" value="<UserID>"></td>
                                            <td><input type="text" class="txtBox w100Per_10" value="사용자ID"></td>
                                        </tr>
                                        <tr>
                                            <td>2</td>
                                            <td><input type="text" class="txtBox w100Per" value="<Pass>"></td>
                                            <td><input type="text" class="txtBox w100Per_10" value="비밀번호"></td>
                                        </tr> --%>
                                    </tbody>
                                </table>
                            </div> 
						</li>
					</ul>
				</div>
				<div class="bu_wrap">
					<button type="button" class="" data-i18n="button.save" onclick="saveFileZone()">저장</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="closePopup()">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/fileTransferZoneSetting.js' />"></script>