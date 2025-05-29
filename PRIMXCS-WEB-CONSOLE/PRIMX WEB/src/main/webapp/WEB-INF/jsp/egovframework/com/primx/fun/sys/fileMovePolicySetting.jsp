<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.moveFilePolicySettings">PRIMX | 이동파일 정책 설정</title>
<form class="fileMovePolicySetting" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.moveFilePolicySettings"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.setting"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.moveFilePolicySettings"></span>
			</h2>
		</div>
	</div>
	<ul class="setWrap">
	    <li class="line">
	        <div class="item">
	            <em data-i18n="moveFilePolicySettings.fileType">파일 종류</em>
	            <div>
		            <div class="selectBox">
						<select class="drop" name="fileType" id="fileType">
							<option data-i18n="moveFilePolicySettings.moveFile" value="M">이동파일</option>
							<!-- <option data-i18n="moveFilePolicySettings.regularFile" value="C">일반파일</option> -->
						</select>
					</div>
	            </div>
	        </div>
	    </li>
	    <li class="line">
	        <div class="item">
	            <em data-i18n="moveFilePolicySettings.validityPeriod">유효기간</em>
	            <div>
	                <input type="text" class="txtBox w200 mr00" value="" id="expiredDate">
	                <span class="minTxt" data-i18n="moveFilePolicySettings.days">일</span>
	            </div>
	        </div>
	    </li>
	    <li class="line">
	        <div class="item">
	            <em data-i18n="moveFilePolicySettings.decryptionCount">복호화 횟수</em>
	            <div>
	                <input type="text" class="txtBox w200 mr00" value="" id="decCount">
	                <span class="minTxt" data-i18n="moveFilePolicySettings.times">회</span>
	            </div>
	        </div>
	    </li>
	    <li class="line">
	        <div class="item">
	            <em data-i18n="moveFilePolicySettings.certificateServerAddress">인증서버 주소</em>
	            <div>
	                <input type="text" class="txtBox" value="" id="certUrl">
	            </div>
	        </div>
	    </li>
	</ul>
	<div class="bu_wrap">
		<button type="button" data-i18n="button.save" onclick="fnSaveFileMovePolicySetting()">저장</button>
	</div>
</form>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/sys/fileMovePolicySetting.js' />"></script>