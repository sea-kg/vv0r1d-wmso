/** Модалка входа + авторизация */

import { RoomAPI } from '../api.js';
import { state } from '../state.js';
import { toast, $ } from '../utils.js';

const loginModal = $('loginModal');
const loginBtn = $('loginBtn');
const loginError = $('loginError');
const usernameInput = $('loginUsername');
const passwordInput = $('loginPassword');

export function showLogin() {
  loginModal.classList.add('visible');
  $('appContainer').hidden = true;
}

export function hideLogin() {
  loginModal.classList.remove('visible');
  $('appContainer').hidden = false;
}

export async function performLogin() {
  const u = usernameInput.value.trim();
  const p = passwordInput.value.trim();
  if (!u || !p) {
    loginError.textContent = 'Заполните оба поля';
    return;
  }
  loginBtn.disabled = true;
  loginBtn.textContent = 'Вход...';
  loginError.textContent = '';
  try {
    await RoomAPI.login(u, p);
    hideLogin();
    state.emit('auth:success');
  } catch (e) {
    loginError.textContent = e.message;
  } finally {
    loginBtn.disabled = false;
    loginBtn.textContent = 'Войти';
  }
}

export async function performLogout() {
  if (state.hasUnsavedChanges) {
    if (!confirm('Есть несохранённые изменения. Выйти без сохранения?')) return;
  }
  try {
    await RoomAPI.logout();
  } catch {}
  state.rooms = [];
  state.currentRoomId = null;
  state.markSaved();
  showLogin();
  state.emit('auth:logout');
}

export function initModal() {
  loginBtn.addEventListener('click', performLogin);
  usernameInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') performLogin();
  });
  passwordInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') performLogin();
  });
  $('logoutBtn').addEventListener('click', performLogout);
}