/* Copyright (C) 2011 Rainmeter Project Developers
 *
 * This Source Code Form is subject to the terms of the GNU General Public
 * License; either version 2 of the License, or (at your option) any later
 * version. If a copy of the GPL was not distributed with this file, You can
 * obtain one at <https://www.gnu.org/licenses/gpl-2.0.html>. */

#include "StdAfx.h"
#include "Player.h"
#include "Internet.h"
#include "Lyrics.h"

/*
** Download lyrics from various serivces.
**
*/
bool Lyrics::GetFromInternet(const std::wstring& artist, const std::wstring& title, std::wstring& out)
{
	std::wstring encArtist = Internet::EncodeUrl(artist);
	std::wstring encTitle = Internet::EncodeUrl(title);

	bool found = GetFromWikia(encArtist, encTitle, out) ||
				 GetFromLetras(encArtist, encTitle, out);

	return found;
}

/*
** Download lyrics from LyricWiki.
**
*/
bool Lyrics::GetFromWikia(const std::wstring& artist, const std::wstring& title, std::wstring& data)
{
	bool ret = false;
	
	std::wstring url = L"http://lyrics.wikia.com/api.php?func=getSong&fmt=json&artist=" + artist;
	url += L"&song=";
	url += title;

	data = Internet::DownloadUrl(url, CP_UTF8);
	if (!data.empty())
	{
		// First we get the URL to the actual wiki page
		std::wstring::size_type pos = data.find(L"http://");
		if (pos != std::wstring::npos)
		{
			data.erase(0, pos);
			pos = data.find_first_of(L'\'');
			url.assign(data, 0, pos);

			// Fetch the wiki page
			data = Internet::DownloadUrl(url, CP_UTF8);
			if (!data.empty())
			{
				pos = data.find(L"'lyricbox'");
				pos = data.find(L"&#", pos);
				if (pos != std::wstring::npos)
				{
					// Get and decode lyrics
					data.erase(0, pos);
					pos = data.find(L"<!");
					data.resize(pos);
					Internet::DecodeReferences(data);

					pos = data.find(L"[...]");
					if (pos != std::wstring::npos)
					{
						// Skip incomplete lyrics
						return ret;
					}

					pos = data.find(L"<p>");
					if (pos != std::wstring::npos)
					{
						// Skip unavailable lyrics
						return ret;
					}

					while ((pos = data.find(L"<br />"), pos) != std::wstring::npos)
					{
						data.replace(pos, 6, L"\n");
					}

					pos = data.find(L"<div class='lyricsbreak'>");
					data.resize(pos);

					// Get rid of all HTML tags
					std::wstring::size_type len = 0;
					while ((pos = data.find_first_of(L'<'), pos) != std::wstring::npos)
					{
						len = data.find_first_of(L'>', pos);
						len -= pos;
						data.erase(pos, ++len);
					}

					ret = true;
				}
			}
		}
	}

	return ret;
}

/*
** Download lyrics from Letras.
**
*/
bool Lyrics::GetFromLetras(const std::wstring& artist, const std::wstring& title, std::wstring& data)
{
	bool ret = false;

	std::wstring url = L"https://www.letras.mus.br/winamp.php?musica=" + title;
	url += L"&artista=";
	url += artist;
	data = Internet::DownloadUrl(url, CP_ACP);
	if (!data.empty())
	{
		std::wstring::size_type pos = data.find(L"\"letra-cnt\"");
		pos = data.find(L"<p>", pos);
		if (pos != std::wstring::npos)
		{
			pos += 6;
			data.erase(0, pos);

			pos = data.find(L"</div>");
			pos -= 9;				
			data.resize(pos);

			Internet::DecodeReferences(data);

			while ((pos = data.find(L"<br/>"), pos) != std::wstring::npos)
			{
				data.replace(pos, 5, L"\n");
			}

			while ((pos = data.find(L"</p><p>"), pos) != std::wstring::npos)
			{
				data.replace(pos, 7, L"\n\n");
			}

			ret = true;
		}
	}

	return ret;
}
